
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
#include "Defines.h"

std::string Character::getMemory(std::vector<Memory *>::iterator &it, std::vector<Memory *> &interval, MemoryFilter &filter) {
    
    std::string ret;
    
    Memory *memory = (Memory *) *it;
    
    std::string pronoum;
    
    if (memory->who != NULL && memory->who == this) {
        pronoum = "I";
    } else if (filter.who == NULL) {
        pronoum = memory->who->name;
    } else if (memory->who->male) {
        pronoum = "He";
    } else {
        pronoum = "She";
    }

    if (memory->event == EventEnteredRoom) {
        
        std::vector<Memory *>::iterator endIt = findEndMemory(it, interval);
        
        if (endIt != interval.end()) {
            
            Memory *endMemory = (Memory *) *endIt;
            
            ret.append(pronoum);
            ret.append(" was in the ");
            ret.append(memory->where->name);
            ret.append(" from around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(" to ");
            ret.append(timeToString(endMemory->when + START_TIME, false));
            ret.append(". ");
            
            ++it;
            
            while (it < endIt) {
                Memory *other = (Memory *) *it;
                if (other->where == memory->where) {
                    ret.append(getMemory(it, interval, filter));
                }
                ++it;
            }
            
        } else {
            
            ret.append(pronoum);
            ret.append(" entered the ");
            ret.append(memory->where->name);
            ret.append(" around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(".");
        }
        
    } else if (memory->event == EventLeftRoom) {
        
        ret.append(pronoum);
        ret.append(" left the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(".");
        
    } else if (memory->event == EventStartInteractPOI) {
        
        std::vector<Memory *>::iterator endIt = findEndMemory(it, interval);
        
        if (endIt != interval.end()) {
            
            Memory *endMemory = (Memory *) *endIt;
            
            ret.append(pronoum);
            ret.append(" looked at the ");
            ret.append(memory->what->description);
            ret.append(" from around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(" to ");
            ret.append(timeToString(endMemory->when + START_TIME, false));
            ret.append(". ");
            
            it = endIt;
            
        } else {
            
            ret.append(pronoum);
            ret.append(" started looking at the ");
            ret.append(memory->what->description);
            ret.append(" in the ");
            ret.append(memory->where->name);
            ret.append(" around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(". ");
        }
        
    } else if (memory->event == EventEndInteractPOI) {
        
        ret.append(pronoum);
        ret.append(" stopped looking at the ");
        ret.append(memory->what->description);
        ret.append(" in the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
        
    } else if (memory->event == EventStartConversation) {
        
        std::vector<Memory *>::iterator endIt = findEndMemory(it, interval);
        
        std::string otherName = memory->whoElse->name;
        if (memory->whoElse == this) {
            otherName = "me";
        }
        
        if (endIt != interval.end()) {
            
            Memory *endMemory = (Memory *) *endIt;
            
            ret.append(pronoum);
            ret.append(" was talking to ");
            ret.append(otherName);
            ret.append(" from around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(" to ");
            ret.append(timeToString(endMemory->when + START_TIME, false));
            ret.append(". ");
            
            it = endIt;
            
        } else {
            
            ret.append(pronoum);
            ret.append(" started talking to ");
            ret.append(otherName);
            ret.append(" around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(". ");
        }
        
    } else if (memory->event == EventEndConversation) {
        
        std::string otherName = memory->whoElse->name;
        if (memory->whoElse == this) {
            otherName = "me";
        }
        
        ret.append(pronoum);
        ret.append(" finished talking to ");
        ret.append(otherName);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
    }
    
    return ret;
}

std::vector<Memory *>::iterator Character::findEndMemory(std::vector<Memory *>::iterator startIt, std::vector<Memory *> &interval) {
    
    Memory *first = (Memory *) *startIt;
    
    Event endEvent;
    
    if (first->event == EventEnteredRoom) {
        endEvent = EventLeftRoom;
    } else if (first->event == EventStartInteractPOI) {
        endEvent = EventEndInteractPOI;
    } else if (first->event == EventStartConversation) {
        endEvent = EventEndConversation;
    }
    
    ++startIt;
    
    while (startIt < interval.end()) {
        
        Memory *cur = (Memory *) *startIt;
        
        if (cur->event == endEvent && cur->what == first->what && cur->where == first->where && cur->who == first->who) {
            
            break;        
        }
        
        ++startIt;
    }
    
    return startIt;
}

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
    
     m->index = memories.size();
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

std::vector<std::string> Character::getMemories(MemoryFilter filter) {
    
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
            
    for (it = interval.begin(); it < interval.end(); ++it) {
        
        std::string line = getMemory(it, interval, filter);
        printf("%s\n\n", line.c_str());
        
        ret.push_back(line);
    }
    
    return ret;
}
