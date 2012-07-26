
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

#ifndef LPC2012Game_Memory_h
#define LPC2012Game_Memory_h

class Character;
class Room;
class POI;

typedef enum tEvent {
    EventEnteredRoom,
    EventWasInRoom,
    EventLeftRoom,
    EventStartInteractPOI,
    EventWasInteractingPOI,
    EventEndInteractPOI,
    EventSawWeapon,
    EventWeaponMissing,
    EventStartConversation,
    EventWasHavingConversation,
    EventEndConversation,
    EventFoundBody,
    EventAll
} Event;

struct MemoryFilter {
    
    long timeStart;
    
    long timeEnd;
    
    Character *who;
    
    Room *where;
    
    bool strange;
};

class Memory {

public:
    
    int index;
    
    Event event;
    
    long when;
    
    Character *who;
    
    Character *whoElse;
    
    Room *where;
    
    POI *what;
    
    POI *whatInside;
    
    int suspicion;
};

#endif
