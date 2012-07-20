
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

#ifndef LPC2012Game_Character_h
#define LPC2012Game_Character_h

#include <string>
#include <vector>

#include "Geometry.h"

#include "Memory.h"
#include "POI.h"

class Character;

typedef enum tStepType {
    StepTypeWalk = 0,
    StepTypeStartInteractPOI,
    StepTypeInteractPOI,
    StepTypeEndInteractPOI,
    StepTypeStartConversation,
    StepTypeConversation,
    StepTypeEndConversation,
    StepTypeAll
} StepType;

struct Step {
    
    Point position;
    long duration;
    StepType type;
    Character *conversationWith;
};

class Character {

private:
    
    std::vector<Memory *> memories;
    
    std::vector<Step *> path;
    
    std::string getMemory(std::vector<Memory *>::iterator &it, std::vector<Memory *> &interval, MemoryFilter &filter);
    
    std::vector<Memory *>::iterator findEndMemory(std::vector<Memory *>::iterator startIt, std::vector<Memory *> &interval);
    
public:
    
    int tag;
    
    Point position;
    
    POI *currentTarget;
    
    Character *murderTarget;
    
    bool idle;    
    
    std::string name;
    
    bool male;
            
    Interest interest;
    
    Interest weaponInterest;
    
    POI *carryingWeapon;
    
    long timeBeforeSearchWeapon;
    
    long timeBeforeTryMurder;
    
    long conversationInterval;
    
    Room *currentRoom;
    
    bool dead;
            
    ~Character();
    
    void addMemory(Memory *m);
    
    void addStep(Step *s);
    
    Step *getCurrentStep();
    
    void updatePath();
    
    void clearPath();
    
    bool isHavingConversation();
    
    bool isInteractingWithPOI();
    
    std::vector<std::string> getMemories(MemoryFilter filter);
};

#endif
