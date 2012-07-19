
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
#include "Room.h"
#include "Utils.h"

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

std::vector<std::string> Character::getMemories(MemoryFilter filter, long startTime) {
    
    std::vector<Memory *> interval;
    std::vector<Memory *>::iterator it;
    
    for (it = memories.begin(); it < memories.end(); ++it) {
        
        Memory *memory = (Memory *) *it;
                        
        if (memory->when >= filter.timeStart && memory->when <= filter.timeEnd) {
            
            bool shouldAdd = false;
            
            if (filter.where == NULL) {
                
                if (memory->who == filter.who) {
                    shouldAdd = true;
                }
            }
            
            if (filter.who == NULL) {
                if (memory->where == filter.where && (memory->event == EventEnteredRoom || memory->event == EventLeftRoom)) {
                    shouldAdd = true;
                }
            }
                        
            if (shouldAdd) {
                interval.push_back(memory);
            }
        }
    }
    
    std::vector<std::string> ret;
    
    for (int i = 0; i < interval.size(); ++i) {
        
        Memory *memory = interval[i];
        
        if (memory->event == EventEnteredRoom) {
            
            int j = i + 1;
            Memory *leftRoom = NULL;
            
            while (j < interval.size() && leftRoom == NULL) {
                
                if (interval[j]->event == EventEnteredRoom) {
                    break;
                }
                
                if (interval[j]->event == EventLeftRoom && interval[j]->where == memory->where) {
                    leftRoom = interval[j];
                } else {
                    j++;
                }
            }
            
            if (leftRoom) {
                
                std::string mem;
                mem.append(memory->who->name);
                mem.append(" was in the ");
                mem.append(memory->where->name);
                mem.append(" from around ");
                mem.append(timeToString(memory->when + startTime, false));
                mem.append(" to ");
                mem.append(timeToString(leftRoom->when + startTime, false));
                mem.append(".");
                
                ret.push_back(mem);
                
                i = j;
                
            } else {
                
                std::string mem;
                mem.append(memory->who->name);
                mem.append(" entered the ");
                mem.append(memory->where->name);
                mem.append(" around ");
                mem.append(timeToString(memory->when + startTime, false));
                mem.append(".");
                
                ret.push_back(mem);
            }
            
        } else if (memory->event == EventLeftRoom) {
            
            std::string mem;
            mem.append(memory->who->name);
            mem.append(" left the ");
            mem.append(memory->where->name);
            mem.append(" around ");
            mem.append(timeToString(memory->when + startTime, false));
            mem.append(".");
            
            ret.push_back(mem);
        }
    }
    
    return ret;
}
