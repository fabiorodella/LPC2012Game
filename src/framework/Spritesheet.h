
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

#ifndef LPC2012Game_Spritesheet_h
#define LPC2012Game_Spritesheet_h

#include <allegro5/allegro.h>

#include "Drawable.h"

class Spritesheet : public Drawable {
    
    Size frameSize;
        
    int numFrames;
           
    int frame;
    
public:
    
    ALLEGRO_BITMAP *image;
    
    Spritesheet(char const *file);
    Spritesheet(char const *file, int fw, int fh);
    ~Spritesheet();
    
    virtual void draw();
    
    void setFrame(int frame);
    int getFrame();
};

#endif
