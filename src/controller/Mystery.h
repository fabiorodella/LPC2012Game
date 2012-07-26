
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

class Mystery {

private:
    
    std::vector<Character *> characters;
    std::vector<Room *> rooms;
    
    long time;
    
    POI *parsePOINode(xmlNode *node);
    Character *parseCharacterNode(xmlNode *node);
    void registerEventFor(Character *target, Event event, Character *who, Character *whoElse, Room *where, POI *what, POI *whatInside);
    void registerEventForAllInRoom(Event event, Character *who, Character *whoElse, Room *where, POI *what, POI *whatInside);
    
public:
    
    short *mapData;
    int mapWidth;
    int mapHeight;
    
    Character *murderer;
    Character *victim;
    
    std::vector<POI *> weapons;
    POI * crimeWeapon;
    
    bool corpseFound;
    long corpseFoundTime;
    Room *corpseFoundRoom;
    bool ended;
    
    Mystery(const char *file, unsigned int seed, short *collisionData, int mapWidth, int mapHeight);
    ~Mystery();
    
    bool isCollision(int x, int y);
            
    void step();
    
    std::vector<Character *> getCharacters();
    std::vector<Room *> getRooms();
};


#endif
