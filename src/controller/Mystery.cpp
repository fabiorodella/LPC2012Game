
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
    
    std::vector<xmlNode *> childNodes = xmlGetChildrenForName(node, "pointOfInterest");
    std::vector<xmlNode *>::iterator it;
    
    for (it = childNodes.begin(); it < childNodes.end(); ++it) {
        
        xmlNode *childNode = (xmlNode *) *it;
        
        POI *child = parsePOINode(childNode);
        poi->contents.push_back(child);
    }
    
    return poi;
}

Mystery::Mystery(const char *file, unsigned int seed, short *collisionData, int mapWidth, int mapHeight) {
    
    srand(seed);
    
    murderHappened = false;
    
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
    
    for (int i = 0; i < numChars; i++) {
        
        Character *character = new Character();
        
        character->tag = i + 1;
        character->male = rand() % 2 == 0;
                        
        character->name = generateName(character->male);
        character->interest = (Interest) (rand() % InterestContainer);
        character->position = pointMake(i + 1, 1);
        character->currentRoom = firstRoom;
        character->idle = true;
        character->currentTarget = NULL;
        character->murderTarget = NULL;
        character->carryingWeapon = NULL;
        character->conversationInterval = CONVERSATION_INTERVAL;
        
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
            case InterestFood:
                strInt = "food";
                break;
            case InterestGames:
                strInt = "games";
            default:
                break;
        }
        
        printf("%s interested in %s\n", character->name.c_str(), strInt);
        
        characters.push_back(character);
    }
    
    int murderTargetIdx = rand() % characters.size();
    Character *murderTarget = characters[murderTargetIdx];
    
    printf("*** %s is the murder target! ***\n", murderTarget->name.c_str());
    
    int murdererIdx = rand() % characters.size();
    Character *murderer = characters[murdererIdx];
    
    while (murderer == murderTarget) {
        murdererIdx = rand() % characters.size();
        murderer = characters[murdererIdx];
    }
    
    //TODO randomize weapon types
    murderer->weaponInterest = InterestWeaponCutting;
    murderer->timeBeforeSearchWeapon = rand() % MAX_DURATION_BEFORE_SEARCH_WEAPON + MIN_DURATION_BEFORE_SEARCH_WEAPON;
    murderer->timeBeforeTryMurder = rand() % MAX_DURATION_BEFORE_TRY_MURDER + MIN_DURATION_BEFORE_TRY_MURDER;
    
    printf("*** %s is the murderer! ***\n", murderer->name.c_str());
    murderer->murderTarget = murderTarget;
    murderer->interest = murderTarget->interest;
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
    
    AStarSearch<MapSearchNode> astarsearch;
      
    std::vector<Character *>::iterator itChars;
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        
        Character *character = (Character *) *itChars;
        
        if (character->idle) {
            
            int targetX = 0;
            int targetY = 0;
            
            // Does the character want to chat?
            bool wantsToTalk = rand() % 2 == 0;
            
            if (character->murderTarget != NULL && character->carryingWeapon != NULL && character->timeBeforeTryMurder == 0) {
                
                // Murderer goes after its target
                    
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
                
                Interest interest = character->interest;
                if (character->murderTarget != NULL && character->carryingWeapon == NULL && character->timeBeforeSearchWeapon == 0) {
                    
                    // Murderer goes after its weapon of choice
                    
                    interest = character->weaponInterest;
                }
                
                do {
                    int roomIdx = rand() % rooms.size();
                    room = rooms[roomIdx];
                    points = room->getPointsOfInterest(interest);
                    
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
                    step->conversation = false;
                    
                    if (character->currentTarget != NULL && pointEqualsIntegral(step->position, character->currentTarget->position)) {
                        
                        // Stops to appreciate/interact with a POI
                        step->duration = rand() % MAX_DURATION_POI_INTERACTION + MIN_DURATION_POI_INTERACTION;
                        
                    } else {
                        step->duration = STEP_DURATION;
                    }
                    
                    character->addStep(step);
                    
                    node = astarsearch.GetSolutionNext();
                }
                
                astarsearch.FreeSolutionNodes();
            }
            
        } else {
            
            // Updates the character's path by 1 unit of time
        
            character->updatePath();
            
            if (!character->havingConversation() && character->conversationInterval > 0) {
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
                
                printf("%s left %s\n", character->name.c_str(), character->currentRoom->name.c_str());
                character->currentRoom = currentRoom;
                printf("%s entered %s\n", character->name.c_str(), character->currentRoom->name.c_str());
            }
            
            bool aloneInRoom = true;
            bool aloneInRoomWithVictim = true;
            
            std::vector<Character *>::iterator itOthers;
            for (itOthers = characters.begin(); itOthers < characters.end(); ++itOthers) {
                Character *other = (Character *) *itOthers;
                
                if (other != character) {
                    if (character->currentRoom == other->currentRoom) {
                        aloneInRoom = false;
                        
                        if (other != character->murderTarget) {
                            aloneInRoomWithVictim = false;
                        }
                    }
                    
                    // Looks for:
                    // - adjacent characters
                    // - one of them without a POI in mind
                    // - both not already in a conversation
                    // - both conversation intervals expired
                    
                    if (pointAdjacentIntegral(character->position, other->position) && 
                        (character->currentTarget == NULL || other->currentTarget == NULL) &&
                        (!character->havingConversation() && !other->havingConversation()) &&
                        (character->conversationInterval == 0 && other->conversationInterval == 0)) {
                        
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
                        step->conversation = true;
                        character->addStep(step);
                        
                        step = new Step();
                        step->position = other->position;
                        step->duration = duration;
                        step->conversation = true;
                        other->addStep(step);
                        
                        character->conversationInterval = CONVERSATION_INTERVAL;
                        other->conversationInterval = CONVERSATION_INTERVAL;
                        
                        printf("%s and %s are having a conversation\n", character->name.c_str(), other->name.c_str());
                        
                    }
                }
            }
            
            // Murderer-specific actions
            
            if (character->murderTarget != NULL) {
                
                // Grabs a weapon if:
                // - not carrying a weapon already
                // - near a weapon of choice
                // - interval elapsed
                // - alone in the room
                
                if (character->carryingWeapon == NULL && character->currentTarget != NULL &&
                    pointEqualsIntegral(character->position, character->currentTarget->position) &&
                    character->currentTarget->isWeapon() &&
                    character->timeBeforeSearchWeapon == 0 &&
                    aloneInRoom) {
                    
                    character->carryingWeapon = character->currentTarget;
                    character->currentRoom->removePointOfInterest(character->currentTarget);
                    
                    printf("*** %s got a weapon! ***\n", character->name.c_str());
                }
                
                // Kills the victim if:
                // - already got a weapon
                // - near the victim
                // - interval elapsed
                // - alone in the room with victim
                
                if (character->carryingWeapon != NULL && character->timeBeforeTryMurder == 0 &&
                    pointAdjacentIntegral(character->position, character->murderTarget->position) &&
                    aloneInRoomWithVictim) {
                    
                    printf("*** %s murdered %s! ***\n", character->name.c_str(), character->murderTarget->name.c_str());
                    murderHappened = true;
                }
            }
        }
    }

    astarsearch.EnsureMemoryFreed();
}

std::vector<Character *> Mystery::getCharacters() {
    return characters;
}
