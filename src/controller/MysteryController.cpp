
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

#include "MysteryController.h"
#include "Room.h"

class MapSearchNode
{
public:
    
    MysteryController *controller;
    
	unsigned int x;	 // the (x,y) positions of the node
	unsigned int y;	
	
	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py, MysteryController *mc ) { x=px; y=py; controller=mc; }
    
	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );
    
	void PrintNodeInfo();
};

bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{
    
	// same state in a maze search is simply when (x,y) are the same
	if( (x == rhs.x) &&
       (y == rhs.y) )
	{
		return true;
	}
	else
	{
		return false;
	}
    
}

void MapSearchNode::PrintNodeInfo()
{
	cout << "Node position : (" << x << ", " << y << ")" << endl;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
	float xd = fabs(float(((float)x - (float)nodeGoal.x)));
	float yd = fabs(float(((float)y - (float)nodeGoal.y)));
    
	return xd + yd;
}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{
    
	if( (x == nodeGoal.x) &&
       (y == nodeGoal.y) )
	{
		return true;
	}
    
	return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node )
{
    
	int parent_x = -1; 
	int parent_y = -1; 
    
	if( parent_node )
	{
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}
	
    
	MapSearchNode NewNode;
    
	// push each possible move except allowing the search to go backwards
    
	if(!controller->isCollision(x - 1, y)
       && !((parent_x == x-1) && (parent_y == y))
       ) 
	{
		NewNode = MapSearchNode( x-1, y, controller );
		astarsearch->AddSuccessor( NewNode );
	}	
    
	if(!controller->isCollision(x, y -1)
       && !((parent_x == x) && (parent_y == y-1))
       ) 
	{
		NewNode = MapSearchNode( x, y-1, controller );
		astarsearch->AddSuccessor( NewNode );
	}	
    
	if(!controller->isCollision(x + 1, y)
       && !((parent_x == x+1) && (parent_y == y))
       ) 
	{
		NewNode = MapSearchNode( x+1, y, controller );
		astarsearch->AddSuccessor( NewNode );
	}	
    
    
	if(!controller->isCollision(x, y + 1) 
       && !((parent_x == x) && (parent_y == y+1))
       )
	{
		NewNode = MapSearchNode( x, y+1, controller );
		astarsearch->AddSuccessor( NewNode );
	}	
    
	return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost( MapSearchNode &successor )
{
	return controller->isCollision(x, y) ? 9 : 0;
}

bool MysteryController::isCollision(int x, int y) {
    
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return true;
    } else if (mapData[y * mapWidth + x] > 0) {
        return true;
    } else {
        return false;
    }
}

void MysteryController::generateMystery(unsigned int seed, int numChars, short *collisionData, int mapWidth, int mapHeight) {
    
    srand(seed);
    
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->mapData = collisionData;
    
    Room *room = new Room();
    sprintf(room->name, "Room 1");
    room->bounds = rectMake(1, 1, 15, 10);
        
    for (int i = 0; i < numChars; i++) {
        
        Character *character = new Character();
        
        character->male = rand() % 2 == 0;
        
        if (character->male) {
            sprintf(character->name, "Male %d", i);
        } else {
            sprintf(character->name, "Female %d", i);
        }
        
        int iInt = rand() % InterestAll;
        Interest interest = (Interest) iInt;
        
        character->interest = interest;
        character->attentive = rand() % 2 == 0;
        character->hasWatch = rand() % 2 == 0;
        character->position = pointMake(i + 1, 1);
        character->currentRoom = room;
        character->moving = false;
        
        const char *strInt;
        
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
    
    rooms.push_back(room);
    
    room = new Room();
    sprintf(room->name, "Room 2");
    room->bounds = rectMake(17, 1, 14, 10);
    rooms.push_back(room);
    
    POI *poi = new POI();
    poi->position = pointMake(30, 2);
    poi->interest = InterestFood;
    room->pointsOfInterest.push_back(poi);
    
    room = new Room();
    sprintf(room->name, "Room 3");
    room->bounds = rectMake(1, 12, 15, 11);
    rooms.push_back(room);
    
    poi = new POI();
    poi->position = pointMake(3, 22);
    poi->interest = InterestPaintings;
    room->pointsOfInterest.push_back(poi);
    
    room = new Room();
    sprintf(room->name, "Room 4");
    room->bounds = rectMake(17, 12, 14, 11);
    rooms.push_back(room);
    
    poi = new POI();
    poi->position = pointMake(30, 20);
    poi->interest = InterestGames;
    room->pointsOfInterest.push_back(poi);
    
    AStarSearch<MapSearchNode> astarsearch;
    
    long time = 0;
    
    while (time < 100) {
        
        std::vector<Character *>::iterator itChars;
        for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
            
            Character *character = (Character *) *itChars;
                                                         
            if (!character->moving) {
                
                std::vector<Room *>::iterator itRooms;
                for (itRooms = rooms.begin(); itRooms < rooms.end(); ++itRooms) {
                    room = (Room *) *itRooms;
                    
                    std::vector<POI *> points = room->getPointsOfInterest(character->interest);
                    
                    if (points.size() > 0) {
                        int idx = rand() % points.size();
                        POI *poi = points[idx];
                        
                        character->target = poi;
                        character->moving = true;
                        
                        MapSearchNode nodeStart;
                        nodeStart.x = (int) character->position.x;
                        nodeStart.y = (int) character->position.y;
                        nodeStart.controller = this;
                                                
                        
                        MapSearchNode nodeEnd;
                        nodeEnd.x = (int) poi->position.x;						
                        nodeEnd.y = (int) poi->position.y;
                        nodeEnd.controller = this;
                        
                        astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);
                        
                        unsigned int SearchState;
                        
                        do {
                            
                            SearchState = astarsearch.SearchStep();
                            
                        } while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );
                        
                        if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {
                            
                            MapSearchNode *node = astarsearch.GetSolutionStart();
                            
                            while (node) {
                                
                                Step *step = new Step();
                                step->position = pointMake(node->x, node->y);
                                step->duration = 1;
                                
                                character->addStep(step);
                                
                                node = astarsearch.GetSolutionNext();
                            }
                                                                                    
                            astarsearch.FreeSolutionNodes();
                        }
                    } // if has POIs
                } // for each room
                
            } else {
                
                character->updatePath(1);
                
                Room *currentRoom = NULL;
                
                std::vector<Room *>::iterator itRooms;
                for (itRooms = rooms.begin(); itRooms < rooms.end(); ++itRooms) {
                    room = (Room *) *itRooms;
                    
                    if (rectContainsPoint(room->bounds, character->position)) {
                        currentRoom = room;
                    }
                }
                
                if (currentRoom != character->currentRoom) {
                    
                    printf("%s left %s\n", character->name, character->currentRoom->name);
                    character->currentRoom = currentRoom;
                    printf("%s entered %s\n", character->name, character->currentRoom->name);
                }
            }
        }
        
        time++;
    }
    
    astarsearch.EnsureMemoryFreed();
}

std::vector<Character *> MysteryController::getCharacters() {
    return characters;
}
