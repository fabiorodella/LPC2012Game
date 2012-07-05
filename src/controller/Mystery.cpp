
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
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "stlastar.h"

#include "Mystery.h"
#include "Room.h"

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

Mystery::Mystery(const char *file, unsigned int seed, short *collisionData, int mapWidth, int mapHeight) {
    
    srand(seed);
    
    murderHappened = false;
    
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->mapData = collisionData;
    
    Room *room = new Room();
    sprintf(room->name, "Room 1");
    room->bounds = rectMake(1, 1, 15, 10);
    
    int numChars = 5;
    
    for (int i = 0; i < numChars; i++) {
        
        Character *character = new Character();
        
        character->tag = i + 1;
        character->male = rand() % 2 == 0;
        
        if (character->male) {
            sprintf(character->name, "M%d", i);
        } else {
            sprintf(character->name, "F%d", i);
        }
        
        int iInt = rand() % InterestAll;
        Interest interest = (Interest) iInt;
        
        character->interest = interest;
        character->attentive = rand() % 2 == 0;
        character->hasWatch = rand() % 2 == 0;
        character->position = pointMake(i + 1, 1);
        character->currentRoom = room;
        character->idle = true;
        character->currentTarget = NULL;
        character->murderTarget = NULL;
        
        const char *strInt = NULL;
        
        switch (interest) {
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
        
        printf("%s interested in %s\n", character->name, strInt);
        
        characters.push_back(character);
    }
    
    int murderTargetIdx = rand() % characters.size();
    Character *murderTarget = characters[murderTargetIdx];
    
    printf("*** %s is the murder target! ***\n", murderTarget->name);
    
    int murdererIdx = rand() % characters.size();
    Character *murderer = characters[murdererIdx];
    
    while (murderer == murderTarget) {
        murdererIdx = rand() % characters.size();
        murderer = characters[murdererIdx];
    }
    
    printf("*** %s is the murderer! ***\n", murderer->name);
    murderer->murderTarget = murderTarget;
    murderer->interest = murderTarget->interest;
    
    rooms.push_back(room);
    
    room = new Room();
    sprintf(room->name, "Room 2");
    room->bounds = rectMake(17, 1, 14, 10);
    rooms.push_back(room);
    
    for (int i = 0; i < InterestAll; i++) {
        POI *poi = new POI();
        poi->position = pointMake(30, 2);
        poi->interest = (Interest) i;
        room->pointsOfInterest.push_back(poi);
    }
    
    room = new Room();
    sprintf(room->name, "Room 3");
    room->bounds = rectMake(1, 12, 15, 11);
    rooms.push_back(room);
    
    for (int i = 0; i < InterestAll; i++) {
        POI *poi = new POI();
        poi->position = pointMake(3, 22);
        poi->interest = (Interest) i;
        room->pointsOfInterest.push_back(poi);
    }
    
    room = new Room();
    sprintf(room->name, "Room 4");
    room->bounds = rectMake(17, 12, 14, 11);
    rooms.push_back(room);
    
    for (int i = 0; i < InterestAll; i++) {
        POI *poi = new POI();
        poi->position = pointMake(30, 20);
        poi->interest = (Interest) i;
        room->pointsOfInterest.push_back(poi);
    }

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
            
            if (wantsToTalk) {
                
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
                
                do {
                    int roomIdx = rand() % rooms.size();
                    room = rooms[roomIdx];
                    points = room->getPointsOfInterest(character->interest);
                    
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
                        step->duration = rand() % 30 + 10;
                        
                    } else {
                        step->duration = 1;
                    }
                    
                    character->addStep(step);
                    
                    node = astarsearch.GetSolutionNext();
                }
                
                astarsearch.FreeSolutionNodes();
            }
            
        } else {
            
            // Updates the character's path by 1 unit of time
        
            character->updatePath();
            
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
                
                printf("%s left %s\n", character->name, character->currentRoom->name);
                character->currentRoom = currentRoom;
                printf("%s entered %s\n", character->name, character->currentRoom->name);
            }
            
            std::vector<Character *>::iterator itOthers;
            for (itOthers = characters.begin(); itOthers < characters.end(); ++itOthers) {
                Character *other = (Character *) *itOthers;
                
                // Looks for:
                // - adjacent characters
                // - one of them without a POI in mind
                // - both not already in a conversation
                
                if (pointAdjacentIntegral(character->position, other->position) && 
                    (character->currentTarget == NULL || other->currentTarget == NULL) &&
                    (!character->havingConversation() && !other->havingConversation())) {
                    
                    int duration = rand() % 60 + 60;
                    
                    // If both characters have matching interests, the talk is longer
                    
                    if (character->interest == other->interest) {
                        duration *= 3;
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
                    
                    printf("%s and %s are having a conversation\n", character->name, other->name);
                    
                }
            }
            
            // TODO improve this
            
            /*
            if (character->murderTarget != NULL && character->currentRoom == character->murderTarget->currentRoom) {
                
                std::vector<Character *>::iterator itAlone;
                bool aloneInTheRoom = true;
                
                for (itAlone = characters.begin(); itAlone < characters.end(); ++itAlone) {
                    Character *other = (Character *) *itAlone;
                    if (other->currentRoom == character->currentRoom && other != character && other != character->murderTarget) {
                        aloneInTheRoom = false;
                    }
                }
                
                if (aloneInTheRoom) {
                    printf("*** %s has been murdered! ***\n", character->murderTarget->name);
                    murderHappened = true;
                }
            }
            */
        }
    }

    astarsearch.EnsureMemoryFreed();
}

std::vector<Character *> Mystery::getCharacters() {
    return characters;
}
