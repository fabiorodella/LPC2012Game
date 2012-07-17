
/*
 
 Copyright 2012 Fabio Rodella
 
 This file is part of LPC2012Game.
 
 LPC2012Game is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 LPC2012Game is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with LPC2012Game.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#include <algorithm>
#include <math.h>

#include "stlastar.h"

#include "XmlHelper.h"
#include "Mystery.h"
#include "Room.h"
#include "NameGenerator.h"

class MapSearchNode {

public:
    
    Mystery *mystery;
    
	unsigned int x;
	unsigned int y;	
	
	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py, Mystery *mc ) { x=px; y=py; mystery=mc; }
    
	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );
    
	void PrintNodeInfo();
};

bool MapSearchNode::IsSameState( MapSearchNode &rhs ) {
    
	if( (x == rhs.x) &&
       (y == rhs.y) ) {
		return true;
	} else {
		return false;
	}
    
}

void MapSearchNode::PrintNodeInfo() {
	cout << "Node position : (" << x << ", " << y << ")" << endl;
}

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal ) {
    
	float xd = fabs(float(((float)x - (float)nodeGoal.x)));
	float yd = fabs(float(((float)y - (float)nodeGoal.y)));
    
	return xd + yd;
}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal ) {
    
	if( (x == nodeGoal.x) &&
       (y == nodeGoal.y) ){
		return true;
	}
    
	return false;
}

bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node ) {
    
	int parent_x = -1; 
	int parent_y = -1; 
    
	if( parent_node ) {
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}
	
    
	MapSearchNode NewNode;
    
	if(!mystery->isCollision(x - 1, y)
       && !((parent_x == x-1) && (parent_y == y))
       ) {
		NewNode = MapSearchNode( x-1, y, mystery );
		astarsearch->AddSuccessor( NewNode );
	}	
    
	if(!mystery->isCollision(x, y -1)
       && !((parent_x == x) && (parent_y == y-1))
       ) {
		NewNode = MapSearchNode( x, y-1, mystery );
		astarsearch->AddSuccessor( NewNode );
	}	
    
	if(!mystery->isCollision(x + 1, y)
       && !((parent_x == x+1) && (parent_y == y))
       ) {
		NewNode = MapSearchNode( x+1, y, mystery );
		astarsearch->AddSuccessor( NewNode );
	}	
    
    
	if(!mystery->isCollision(x, y + 1) 
       && !((parent_x == x) && (parent_y == y+1))
       ) {
		NewNode = MapSearchNode( x, y+1, mystery );
		astarsearch->AddSuccessor( NewNode );
	}	
    
	return true;
}

float MapSearchNode::GetCost( MapSearchNode &successor ) {
	return mystery->isCollision(x, y) ? 9 : 0;
}

POI *Mystery::parsePOINode(xmlNode *node) {
    
    POI *poi = new POI();
    
    int x = atoi(xmlGetAttribute(node, "positionX"));
    int y = atoi(xmlGetAttribute(node, "positionY"));
    int interest = atoi(xmlGetAttribute(node, "interest"));
    
    poi->position = pointMake(x, y);
    poi->interest = (Interest) interest;
    poi->description = xmlGetAttribute(node, "description");
    poi->visualPosition = poi->position;
    poi->contents = NULL;
    poi->searchedByMurderer = false;
        
    return poi;
}

void Mystery::registerEventFor(Character *target, Event event, Character *who, Room *where, POI *what) {
    
    Memory *memory = new Memory();
    memory->event = event;
    memory->who = who;
    memory->where = where;
    memory->what = what;
    memory->when = time;
    memory->suspicion = 1;
    
    target->addMemory(memory);
}

void Mystery::registerEventForAllInRoom(Event event, Character *who, Room *where, POI *what) {
    
    std::vector<Character *>::iterator it;
    for (it = characters.begin(); it < characters.end(); ++it) {
        
        Character *character = (Character *) *it;
        
        if (character->currentRoom == where) {
            
            registerEventFor(character, event, who, where, what);
        }
    }
}

Mystery::Mystery(const char *file, unsigned int seed, short *collisionData, int mapWidth, int mapHeight) {
    
    srand(seed);
    
    corpseFound = false;
    ended = false;
    
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->mapData = collisionData;
    
    Room *firstRoom = NULL;
    
    xmlDoc *doc = xmlReadFile(file, NULL, 0);
    
    xmlNode *root = xmlDocGetRootElement(doc);
    
    int numChars = atoi(xmlGetAttribute(root, "numCharacters"));
            
    std::vector<xmlNode *> roomNodes = xmlGetChildrenForName(root, "room");
    std::vector<xmlNode *>::iterator it;
    
    for (it = roomNodes.begin(); it < roomNodes.end(); ++it) {
        
        xmlNode *roomNode = (xmlNode *) *it;
        
        Room *room = new Room();
        room->name = xmlGetAttribute(roomNode, "name");
        
        int ox = atoi(xmlGetAttribute(roomNode, "boundsOriginX"));
        int oy = atoi(xmlGetAttribute(roomNode, "boundsOriginY"));
        int w = atoi(xmlGetAttribute(roomNode, "boundsWidth"));
        int h = atoi(xmlGetAttribute(roomNode, "boundsHeight"));
        
        room->bounds = rectMake(ox, oy, w, h);
        
        if (xmlGetAttribute(roomNode, "firstRoom") != NULL) {
            firstRoom = room;
        }
        
        std::vector<xmlNode *> POINodes = xmlGetChildrenForName(roomNode, "pointOfInterest");
        std::vector<xmlNode *>::iterator itPOI;
        
        for (itPOI = POINodes.begin(); itPOI < POINodes.end(); itPOI++) {
            
            xmlNode *POInode = (xmlNode *) *itPOI;
            
            POI *poi = parsePOINode(POInode);            
            room->pointsOfInterest.push_back(poi);
        }
        
        rooms.push_back(room);
    }
    
    std::vector<xmlNode *> weaponNodes = xmlGetChildrenForName(root, "weapon");
    
    for (it = weaponNodes.begin(); it < weaponNodes.end(); ++it) {
        
        xmlNode *weaponNode = (xmlNode *) *it;
        
        POI *weapon = parsePOINode(weaponNode);
        
        int idx = rand() % rooms.size();
        Room *room = rooms[idx];
                
        std::vector<POI *> containers = room->getPointsOfInterest(InterestContainerVisible, false);
        std::vector<POI *> containers2 = room->getPointsOfInterest(InterestContainerConceiled, false);
        
        containers.insert(containers.end(), containers2.begin(), containers2.end());
        
        idx = rand() % containers.size();
        
        POI *container = containers[idx];
        container->contents = weapon;
        weapon->position = container->visualPosition;
        
        if (container->interest == InterestContainerConceiled) {
            weapon->position = pointMake(-20, -20);
        }
        
        weapons.push_back(weapon);
        
        printf("%s is in the %s in the %s\n", weapon->description.c_str(), container->description.c_str(), room->name.c_str());
    }
    
    for (int i = 0; i < numChars; i++) {
        
        Character *character = new Character();
        
        character->tag = i + 1;
        character->male = rand() % 2 == 0;
                        
        character->name = generateName(character->male);
        character->interest = (Interest) (rand() % InterestContainerVisible);
        character->position = pointMake(i + 35, 50);
        character->currentRoom = firstRoom;
        character->idle = true;
        character->currentTarget = NULL;
        character->murderTarget = NULL;
        character->carryingWeapon = NULL;
        character->conversationInterval = CONVERSATION_INTERVAL;
        character->dead = false;
        
        const char *strInt = NULL;
        
        switch (character->interest) {
            case InterestBooks:
                strInt = "books";
                break;
            case InterestPottery:
                strInt = "pottery";
                break;
            case InterestPaintings:
                strInt = "paintings";
                break;
            default:
                break;
        }
        
        printf("%s interested in %s\n", character->name.c_str(), strInt);
        
        characters.push_back(character);
    }
    
    int murderTargetIdx = rand() % characters.size();
    victim = characters[murderTargetIdx];
    
    printf("*** %s is the murder target! ***\n", victim->name.c_str());
    
    int murdererIdx = rand() % characters.size();
    Character *murderer = characters[murdererIdx];
    
    while (murderer == victim) {
        murdererIdx = rand() % characters.size();
        murderer = characters[murdererIdx];
    }
    
    //TODO randomize weapon types
    murderer->weaponInterest = InterestWeaponCutting;
    murderer->timeBeforeSearchWeapon = rand() % MAX_DURATION_BEFORE_SEARCH_WEAPON + MIN_DURATION_BEFORE_SEARCH_WEAPON;
    murderer->timeBeforeTryMurder = rand() % MAX_DURATION_BEFORE_TRY_MURDER + MIN_DURATION_BEFORE_TRY_MURDER;
    
    printf("*** %s is the murderer! ***\n", murderer->name.c_str());
    murderer->murderTarget = victim;
    
    time = 0;
}

Mystery::~Mystery() {
    
    std::vector<Character *>::iterator itChars;
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        delete *itChars;
    }
    
    characters.clear();
    
    std::vector<Room *>::iterator itRooms;
    for (itRooms = rooms.begin(); itRooms < rooms.end(); ++itRooms) {
        delete *itRooms;
    }
    
    rooms.clear();
    
    weapons.clear();
}

bool Mystery::isCollision(int x, int y) {
    
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return true;
    } else if (mapData[y * mapWidth + x] > 0) {
        return true;
    } else {
        return false;
    }
}

void Mystery::step() {
    
    if (ended) {
        return;
    }
    
    AStarSearch<MapSearchNode> astarsearch;
      
    std::vector<Character *>::iterator itChars;
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        
        Character *character = (Character *) *itChars;
        
        if (character->dead) {
            continue;
        }
        
        if (character->idle) {
            
            int targetX = 0;
            int targetY = 0;
            
            // Any dead people in the same room?
            
            Character *corpseInRoom = NULL;
            
            std::vector<Character *>::iterator itOthers;
            for (itOthers = characters.begin(); itOthers < characters.end(); ++itOthers) {
                Character *other = (Character *) *itOthers;
                if (other->currentRoom == character->currentRoom && other->dead) {
                    corpseInRoom = other;
                }
            }
            
            // Does the character want to chat?
            bool wantsToTalk = rand() % 2 == 0;
            
            if (corpseFound && (character->murderTarget == NULL || (character->murderTarget != NULL && character->carryingWeapon == NULL))) {
                
                targetX = victim->position.x;
                targetY = victim->position.y;
                
                character->currentTarget = NULL;
                
            } else if (corpseInRoom != NULL && character->murderTarget == NULL) {
                
                // Go look at the corpse
                
                targetX = corpseInRoom->position.x;
                targetY = corpseInRoom->position.y;
                
                character->currentTarget = NULL;
                
            } else if (character->murderTarget != NULL && !character->murderTarget->dead && character->carryingWeapon != NULL && character->timeBeforeTryMurder == 0) {
                
                // Murderer goes after its target if he has a weapon and the target is
                // not dead yet
                    
                targetX = character->murderTarget->position.x;
                targetY = character->murderTarget->position.y;
                    
                character->currentTarget = NULL;
            
            } else if (wantsToTalk) {
                
                // Goes after another character
                
                int idxOption = rand() % characters.size();
                Character *option = characters[idxOption];
                targetX = option->position.x;
                targetY = option->position.y;
                
                character->currentTarget = NULL;
            
            } else {
            
                // Looks for a POI matching the characters interest in any room
                
                Room *room;
                std::vector<POI *> points;
                
                bool notSearchedOnly = false;
                
                Interest interest = character->interest;
                if (character->murderTarget != NULL && ((character->carryingWeapon == NULL && character->timeBeforeSearchWeapon == 0 && !character->murderTarget->dead) || (character->carryingWeapon != NULL && character->murderTarget->dead))) {
                    
                    // If the character is a murderer and is looking for a weapon, or
                    // if he/she already killed the target, he/she goes to a container
                    // to grab/hide the weapon
                    
                    interest = rand() % 2 == 0 ? InterestContainerVisible : InterestContainerConceiled;
                    
                    // If he has not weapon, searches only the containers not
                    // searched yet
                    
                    notSearchedOnly = character->carryingWeapon == NULL;
                }
                
                do {
                    int roomIdx = rand() % rooms.size();
                    room = rooms[roomIdx];
                    
                    points = room->getPointsOfInterest(interest, notSearchedOnly);
                    
                } while (points.size() == 0);
                
                // Goes after an interesting random POI
                
                int idx = rand() % points.size();
                POI *poi = points[idx];
                
                character->currentTarget = poi;
                
                targetX = poi->position.x;
                targetY = poi->position.y;
            }
            
            character->idle = false;
            
            MapSearchNode nodeStart;
            nodeStart.x = (int) character->position.x;
            nodeStart.y = (int) character->position.y;
            nodeStart.mystery = this;
            
            MapSearchNode nodeEnd;
            nodeEnd.x = targetX;						
            nodeEnd.y = targetY;
            nodeEnd.mystery = this;
            
            astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);
            
            unsigned int SearchState;
            
            do {
                
                SearchState = astarsearch.SearchStep();
                
            } while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );
            
            // Found a path to the target
            
            if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {
                
                MapSearchNode *node = astarsearch.GetSolutionStart();
                
                while (node) {
                    
                    // Builds the steps for the character to reach its target
                    
                    Step *step = new Step();
                    step->position = pointMake(node->x, node->y);
                    step->conversationWith = NULL;
                    step->type = StepTypeWalk;
                    step->duration = STEP_DURATION;
                    
                    if (character->currentTarget != NULL && pointEqualsIntegral(step->position, character->currentTarget->position)) {
                        
                        Step *step = new Step();
                        step->position = pointMake(node->x, node->y);
                        step->conversationWith = NULL;
                        step->type = StepTypeStartInteractPOI;
                        step->duration = 2;
                        
                        character->addStep(step);
                        
                        step = new Step();
                        step->position = pointMake(node->x, node->y);
                        step->conversationWith = NULL;
                        step->type = StepTypeInteractPOI;
                        step->duration = rand() % MAX_DURATION_POI_INTERACTION + MIN_DURATION_POI_INTERACTION;
                        
                        character->addStep(step);
                        
                        step = new Step();
                        step->position = pointMake(node->x, node->y);
                        step->conversationWith = NULL;
                        step->type = StepTypeEndInteractPOI;
                        step->duration = 2;
                        
                        character->addStep(step);
                        
                    } else {
                        
                        Step *step = new Step();
                        step->position = pointMake(node->x, node->y);
                        step->conversationWith = NULL;
                        step->type = StepTypeWalk;
                        step->duration = STEP_DURATION;
                        
                        character->addStep(step);
                    }
                    
                    node = astarsearch.GetSolutionNext();
                }
                
                astarsearch.FreeSolutionNodes();
            }
            
        } else {
            
            // Updates the character's path by 1 unit of time
        
            character->updatePath();
            
            if (!character->isHavingConversation() && character->conversationInterval > 0) {
                character->conversationInterval--;
            }
            
            if (character->timeBeforeSearchWeapon > 0) {
                character->timeBeforeSearchWeapon--;
            } else {
                
                if (character->timeBeforeTryMurder > 0 && character->carryingWeapon != NULL) {
                    character->timeBeforeTryMurder--;
                }
            }
            
            Room *currentRoom = NULL;
            
            // Determines if the character entered or left a room
            
            std::vector<Room *>::iterator itRooms;
            for (itRooms = rooms.begin(); itRooms < rooms.end(); ++itRooms) {
                Room *room = (Room *) *itRooms;
                
                if (rectContainsPoint(room->bounds, character->position)) {
                    currentRoom = room;
                }
            }
            
            if (currentRoom != character->currentRoom) {
                
                if (character->currentRoom != NULL) {
                    printf("%s left %s\n", character->name.c_str(), character->currentRoom->name.c_str());
                    registerEventForAllInRoom(EventLeftRoom, character, character->currentRoom, NULL);
                }
                
                character->currentRoom = currentRoom;
                
                if (character->currentRoom != NULL) {
                    printf("%s entered %s\n", character->name.c_str(), character->currentRoom->name.c_str());
                    registerEventForAllInRoom(EventEnteredRoom, character, character->currentRoom, NULL);
                    
                    // Register visible weapons the character saw in the room
                    std::vector<POI *>::iterator itPOI;
                    for (itPOI = character->currentRoom->pointsOfInterest.begin(); itPOI < character->currentRoom->pointsOfInterest.end(); ++itPOI) {
                        
                        POI *poi = (POI *) *itPOI;
                        
                        if (poi->interest == InterestContainerVisible && poi->contents != NULL && poi->contents->isWeapon()) {
                            registerEventFor(character, EventSawWeapon, NULL, character->currentRoom, poi);
                        }
                    }
                    
                    std::vector<Character *>::iterator itOthers;
                    for (itOthers = characters.begin(); itOthers < characters.end(); ++itOthers) {
                        Character *other = (Character *) *itOthers;
                        
                        if (other != character) {
                            
                            // Register others having conversations
                            if (other->isHavingConversation()) {
                                registerEventFor(character, EventStartConversation, other, character->currentRoom, NULL);
                            }
                            
                            // Register others interacting with POIs
                            if (other->isInteractingWithPOI()) {
                                registerEventFor(character, EventStartInteractPOI, NULL, character->currentRoom, other->currentTarget);
                            }
                        }
                    }
                }
            }
            
            if (character->getCurrentStep() != NULL) {
                
                switch (character->getCurrentStep()->type) {
                    case StepTypeStartInteractPOI:
                        registerEventForAllInRoom(EventStartInteractPOI, character, character->currentRoom, character->currentTarget);
                        break;
                    case StepTypeEndInteractPOI:
                        registerEventForAllInRoom(EventEndInteractPOI, character, character->currentRoom, character->currentTarget);
                        break;
                    case StepTypeEndConversation:
                        registerEventForAllInRoom(EventEndConversation, character, character->currentRoom, character->currentTarget);
                        break;
                    default:
                        break;
                }
            }
            
            bool aloneInRoom = true;
            bool aloneInRoomWithVictim = true;
            bool allNearCorpse = pointEqualsIntegral(character->position, victim->position);
            
            std::vector<Character *>::iterator itOthers;
            for (itOthers = characters.begin(); itOthers < characters.end(); ++itOthers) {
                Character *other = (Character *) *itOthers;
                
                allNearCorpse = allNearCorpse && pointEqualsIntegral(other->position, victim->position);
                
                if (other != character) {
                    if (character->currentRoom == other->currentRoom) {
                        aloneInRoom = false;
                        
                        if (other != character->murderTarget) {
                            aloneInRoomWithVictim = false;
                        }
                    }
                    
                    if (victim->dead && other == victim && pointEqualsIntegral(character->position, other->position) && !corpseFound) {
                        
                        printf("*** %s found %s's body in the %s ***\n", character->name.c_str(), victim->name.c_str(), character->currentRoom->name.c_str());
                        corpseFound = true;
                        character->clearPath();
                    }
                    
                    // Looks for:
                    // - adjacent characters
                    // - one of them without a POI in mind
                    // - both not already in a conversation
                    // - both conversation intervals expired
                    
                    if (pointAdjacentIntegral(character->position, other->position) && 
                        (character->currentTarget == NULL || other->currentTarget == NULL) &&
                        (!character->isHavingConversation() && !other->isHavingConversation()) &&
                        (character->conversationInterval == 0 && other->conversationInterval == 0) && !corpseFound) {
                        
                        int duration = rand() % MAX_DURATION_CONVERSATION + MIN_DURATION_CONVERSATION;
                        
                        // If both characters have matching interests, the talk is longer
                        
                        if (character->interest == other->interest) {
                            duration *= CONVERSATION_INTEREST_FACTOR;
                        }
                        
                        character->clearPath();
                        other->clearPath();
                        
                        Step *step = new Step();
                        step->position = character->position;
                        step->duration = duration;
                        step->conversationWith = other;
                        step->type = StepTypeConversation;
                        character->addStep(step);
                        
                        step = new Step();
                        step->position = character->position;
                        step->duration = 2;
                        step->conversationWith = other;
                        step->type = StepTypeEndConversation;
                        character->addStep(step);
                        
                        step = new Step();
                        step->position = other->position;
                        step->duration = duration;
                        step->conversationWith = character;
                        step->type = StepTypeConversation;
                        other->addStep(step);
                        
                        step = new Step();
                        step->position = other->position;
                        step->duration = 2;
                        step->conversationWith = character;
                        step->type = StepTypeEndConversation;
                        other->addStep(step);
                        
                        character->conversationInterval = CONVERSATION_INTERVAL;
                        other->conversationInterval = CONVERSATION_INTERVAL;
                        
                        registerEventForAllInRoom(EventStartConversation, character, character->currentRoom, NULL);
                        registerEventForAllInRoom(EventEndConversation, other, other->currentRoom, NULL);
                        
                        printf("%s and %s are having a conversation\n", character->name.c_str(), other->name.c_str());
                        
                    }
                }
            }
            
            if (allNearCorpse && corpseFound) {
                ended = true;
                printf("*** Mystery finished ***\n");
            }
            
            // Murderer-specific actions
            
            if (character->murderTarget != NULL) {
                
                // Grabs a weapon if:
                // - interval elapsed
                // - not carrying a weapon already
                // - murder target is not dead
                // - has a target POI
                // - reached the target POI
                // - POI has contents
                // - POI's content is a weapon matching interest
                // - alone in the room
                
                if (character->timeBeforeSearchWeapon == 0 &&
                    character->carryingWeapon == NULL &&
                    !victim->dead &&
                    character->currentTarget != NULL &&
                    pointEqualsIntegral(character->position, character->currentTarget->position) &&
                    character->currentTarget->contents != NULL &&
                    character->currentTarget->contents->interest == character->weaponInterest &&
                    aloneInRoom) {
                    
                    character->carryingWeapon = character->currentTarget->contents;
                    character->currentTarget->contents = NULL;
                    
                    printf("*** %s got a %s! ***\n", character->name.c_str(), character->carryingWeapon->description.c_str());
                }
                
                // Kills the victim if:
                // - interval elapsed
                // - already got a weapon
                // - near the victim
                // - murder target is not dead
                // - alone in the room with victim
                
                if (character->timeBeforeTryMurder == 0 &&
                    character->carryingWeapon != NULL && 
                    pointAdjacentIntegral(character->position, victim->position) &&
                    !victim->dead &&
                    aloneInRoomWithVictim) {
                    
                    printf("*** %s murdered %s! ***\n", character->name.c_str(), victim->name.c_str());
                    victim->dead = true;
                    
                    character->clearPath();
                }
                
                // Hides the weapon if:
                // - carrying a weapon
                // - murder target is dead
                // - has a target POI
                // - reached the target POI
                // - POI has no contents
                // - alone in the room
                
                if (character->carryingWeapon != NULL && 
                    victim->dead &&
                    character->currentTarget != NULL &&
                    pointEqualsIntegral(character->position, character->currentTarget->position) &&
                    character->currentTarget->contents == NULL &&
                    aloneInRoom) {
                    
                    character->currentTarget->contents = character->carryingWeapon;
                    character->carryingWeapon = NULL;
                    
                    printf("*** %s hid the %s in the %s, in the %s! ***\n", character->name.c_str(), character->currentTarget->contents->description.c_str(), character->currentTarget->description.c_str(), currentRoom->name.c_str());
                }
            }
        }
    }

    astarsearch.EnsureMemoryFreed();
    
    time++;
}

std::vector<Character *> Mystery::getCharacters() {
    return characters;
}

std::vector<Room *> Mystery::getRooms() {
    return rooms;
}
