
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

#include <stdio.h>

#include "Character.h"

Character::~Character() {
    
    std::vector<Memory *>::iterator itMem;
    for (itMem = memories.begin(); itMem < memories.end(); itMem++) {
        delete *itMem;
    }
    
    memories.clear();
    
    std::vector<Step *>::iterator itSteps;
    for (itSteps = path.begin(); itSteps < path.end(); itSteps++) {
        delete *itSteps;
    }
    
    path.clear();
}

void Character::addMemory(Memory *m) {
    memories.push_back(m);
}

void Character::addStep(Step *s) {
    path.push_back(s);
}

Step *Character::getCurrentStep() {
    if (path.size() > 0) {
        return path[0];
    }
    return NULL;
}

void Character::updatePath() {
    
    if (!idle) {
        
        if (path.size() > 0) {
            
            Step *s = path[0];
            
            s->duration -= 1;
            position = s->position;
            
            if (s->duration <= 0) {
                
                path.erase(path.begin());
                delete s;
            }
        }
        
        if (path.size() == 0) {
            idle = true;
        }
    }
}

void Character::clearPath() {
    
    std::vector<Step *>::iterator itSteps;
    for (itSteps = path.begin(); itSteps < path.end(); itSteps++) {
        delete *itSteps;
    }
    
    path.clear();
    idle = true;
}

bool Character::isHavingConversation() {
    if (path.size() > 0) {
        return path[0]->conversationWith != NULL;
    }
    return false;
}

bool Character::isInteractingWithPOI() {
    if (path.size() > 0) {
        return path[0]->duration > 1 && currentTarget != NULL && pointEqualsIntegral(currentTarget->position, position);
    }
    return false;
}
