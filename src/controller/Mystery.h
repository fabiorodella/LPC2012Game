
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

#ifndef LPC2012Game_Mystery_h
#define LPC2012Game_Mystery_h

#include <vector>

#include "Character.h"

#define STEP_DURATION 2
#define MIN_DURATION_BEFORE_SEARCH_WEAPON 200
#define MAX_DURATION_BEFORE_SEARCH_WEAPON 500
#define MIN_DURATION_BEFORE_TRY_MURDER 200
#define MAX_DURATION_BEFORE_TRY_MURDER 500
#define MIN_DURATION_POI_INTERACTION 10
#define MAX_DURATION_POI_INTERACTION 30
#define MIN_DURATION_CONVERSATION 60
#define MAX_DURATION_CONVERSATION 60
#define CONVERSATION_INTEREST_FACTOR 3
#define CONVERSATION_INTERVAL 120

class Mystery {

private:
    
    std::vector<Character *> characters;
    std::vector<Room *> rooms;
    
    long time;
    
    POI *parsePOINode(xmlNode *node);
    void registerEventFor(Character *target, Event event, Character *who, Character *whoElse, Room *where, POI *what);
    void registerEventForAllInRoom(Event event, Character *who, Character *whoElse, Room *where, POI *what);
    
public:
    
    short *mapData;
    int mapWidth;
    int mapHeight;
    
    Character *victim;
    
    std::vector<POI *> weapons;
    
    bool corpseFound;
    bool ended;
    
    Mystery(const char *file, unsigned int seed, short *collisionData, int mapWidth, int mapHeight);
    ~Mystery();
    
    bool isCollision(int x, int y);
            
    void step();
    
    std::vector<Character *> getCharacters();
    std::vector<Room *> getRooms();
};


#endif
