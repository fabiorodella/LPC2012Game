
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

#include "MysteryController.h"

void MysteryController::generateMystery(unsigned int seed, int numChars, short *collisionData) {
    
    srand(seed);
    
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
    }
    
}
