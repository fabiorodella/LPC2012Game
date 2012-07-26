
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
    } else if (memory->who != NULL && filter.who == NULL) {
        pronoum = memory->who->name;
    } else if (memory->who != NULL && memory->who->male) {
        pronoum = "He";
    } else if (memory->who != NULL) {
        pronoum = "She";
    }

    if (memory->event == EventEnteredRoom) {
        
        ret.append(pronoum);
        ret.append(" entered the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(".");
      
    } else if (memory->event == EventWasInRoom) {
        
        ret.append(pronoum);
        ret.append(" was there when I entered the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(".");
        
    } else if (memory->event == EventLeftRoom) {
        
        ret.append(pronoum);
        ret.append(" left the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(".");
        
    } else if (memory->event == EventStartInteractPOI) {
        
        ret.append(pronoum);
        ret.append(" started looking at the ");
        ret.append(memory->what->description);
        ret.append(" in the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
      
    } else if (memory->event == EventWasInteractingPOI) {
        
        ret.append(pronoum);
        ret.append(" was looking at the ");
        ret.append(memory->what->description);
        ret.append(" when I entered the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
        
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
        
        std::string otherName = memory->whoElse->name;
        if (memory->whoElse == this) {
            otherName = "me";
        }
        
        ret.append(pronoum);
        ret.append(" started talking to ");
        ret.append(otherName);
        ret.append(" in the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
    
    } else if (memory->event == EventWasHavingConversation) {
        
        ret.append(pronoum);
        ret.append(" was talking to ");
        ret.append(memory->whoElse->name);
        ret.append(" when I entered the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
        
    } else if (memory->event == EventEndConversation) {
        
        std::string otherName = memory->whoElse->name;
        if (memory->whoElse == this) {
            otherName = "me";
        }
        
        ret.append(pronoum);
        ret.append(" finished talking to ");
        ret.append(otherName);
        ret.append(" in the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
        
    } else if (memory->event == EventSawWeapon) {
        
        ret.append("I saw a ");
        ret.append(memory->whatInside->description);
        ret.append(" on the ");
        ret.append(memory->what->description);
        ret.append(" in the ");
        ret.append(memory->where->name);
        ret.append(" around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
        
    } else if (memory->event == EventWeaponMissing) {
        
        ret.append("I saw a ");
        ret.append(memory->whatInside->description);
        ret.append(" on the ");
        ret.append(memory->what->description);
        ret.append(" in the ");
        ret.append(memory->where->name);
        ret.append(" before, but it wasn't there around ");
        ret.append(timeToString(memory->when + START_TIME, false));
        ret.append(". ");
        
    } else if (memory->event == EventFoundBody) {
        
        if (memory->who == this) {
            
            ret.append("I found a dead body in the ");
            ret.append(memory->where->name);
            ret.append(" around ");
            ret.append(timeToString(memory->when + START_TIME, false));
            ret.append(". ");
            
        } else {
            
            ret.append("I heard ");
            ret.append(memory->who->name);
            ret.append(" screaming from the ");
            ret.append(memory->where->name);
            ret.append(" about finding a dead body. ");
            
        }
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
        
        if (cur->where != first->where) {
            return interval.end();
        }
        
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

std::vector<Memory *> Character::getMemories() {
    return memories;
}

std::vector<std::string> Character::getFormattedMemories(MemoryFilter filter) {
    
    std::vector<Memory *> interval;
    std::vector<Memory *>::iterator it;
    
    for (it = memories.begin(); it < memories.end(); ++it) {
        
        Memory *memory = (Memory *) *it;
                        
        if (memory->when >= filter.timeStart && memory->when <= filter.timeEnd) {
            
            bool shouldAdd = false;
            
            if (filter.where == NULL) {
                
                if (memory->who == filter.who) {
                    shouldAdd = true;
                    
                    if (filter.strange) {
                        
                        if ((memory->what != NULL && memory->what->isContainer()) || memory->event == EventSawWeapon || memory->event == EventWeaponMissing || memory->event == EventFoundBody) {
                            shouldAdd = false;
                        }
                        
                    }
                }
            }
            
            if (filter.who == NULL) {
                
                if (!filter.strange && memory->where == filter.where && (memory->event == EventEnteredRoom || memory->event == EventLeftRoom)) {
                    shouldAdd = true;
                    
                } else if (filter.strange) {
                    
                    if ((memory->what != NULL && memory->what->isContainer()) || memory->event == EventSawWeapon || memory->event == EventWeaponMissing || memory->event == EventFoundBody) {
                        shouldAdd = true;
                    }
                }
            }
                        
            if (shouldAdd) {
                interval.push_back(memory);
                
                std::string log;
                log.append(timeToString(memory->when, true));
                log.append(" ");
                
                switch (memory->event) {
                    case EventEnteredRoom:
                        log.append("EventEnteredRoom ");
                        break;
                    case EventWasInRoom:
                        log.append("EventWasInRoom ");
                        break;
                    case EventLeftRoom:
                        log.append("EventLeftRoom ");
                        break;
                    case EventStartInteractPOI:
                        log.append("EventStartInteractPOI ");
                        break;
                    case EventWasInteractingPOI:
                        log.append("EventWasInteractingPOI ");
                        break;
                    case EventEndInteractPOI:
                        log.append("EventEndInteractPOI ");
                        break;
                    case EventStartConversation:
                        log.append("EventStartConversation ");
                        break;
                    case EventWasHavingConversation:
                        log.append("EventWasHavingConversation ");
                        break;
                    case EventEndConversation:
                        log.append("EventEndConversation ");
                        break;
                    case EventSawWeapon:
                        log.append("EventSawWeapon ");
                        break;
                    case EventWeaponMissing:
                        log.append("EventWeaponMissing ");
                        break;
                    case EventFoundBody:
                        log.append("EventFoundBody ");
                        break;
                    default:
                        break;
                }
                
                log.append("\nWho: ");
                log.append(memory->who != NULL ? memory->who->name : "NULL");
                
                log.append("\nWho else: ");
                log.append(memory->whoElse != NULL ? memory->whoElse->name : "NULL");
                
                log.append("\nWhere: ");
                log.append(memory->where != NULL ? memory->where->name : "NULL");
                
                log.append("\nWhat: ");
                log.append(memory->what != NULL ? memory->what->description : "NULL");
                
                log.append("\nWhat inside: ");
                log.append(memory->whatInside != NULL ? memory->whatInside->description : "NULL");
                
                printf("%s\n\n", log.c_str());
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
