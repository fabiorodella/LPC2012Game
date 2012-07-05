
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

Character::Character() {
    name = new char[100];
}

Character::~Character() {
    delete name;
}

void Character::addMemory(Memory *m) {
    memories.push_back(m);
}

void Character::addStep(Step *s) {
    path.push_back(s);
}

void Character::updatePath(long time) {
    
    if (moving) {
        
        Step *s = path[0];
        
        s->duration -= time;
        
        if (s->duration <= 0) {
            position = s->position;
            path.erase(path.begin());
            
            if (path.size() == 0) {
                moving = false;
            }
        }
    }
}