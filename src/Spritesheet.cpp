
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

#include "Spritesheet.h"

Spritesheet::Spritesheet(char const *file) {
    
    camera = NULL;
    
    image = al_load_bitmap(file);
    
    numFrames = 1;
    
    posX = 0;
    posY = 0;
    
    frame = 0;
    
    frameWidth = al_get_bitmap_width(image);
    frameHeight = al_get_bitmap_height(image);
}

Spritesheet::Spritesheet(char const *file, int fw, int fh) {
    
    camera = NULL;
    
    image = al_load_bitmap(file);
    
    numFrames = 0;
    
    posX = 0;
    posY = 0;
    
    frame = 0;
    
    frameWidth = fw;
    frameHeight = fh;
    
    int x = 0;
    int y = 0;
    
    int w = al_get_bitmap_width(image);
    int h = al_get_bitmap_height(image);
    
    while (y < h) {
        while (x < w) {
            numFrames++;
            x += fw;
        }
        y += fh;
        x = 0;
    }
}

Spritesheet::~Spritesheet() {
    
    al_destroy_bitmap(image);
}

void Spritesheet::draw() {
            
    float px = posX;
    float py = posY;
    
    if (camera != NULL) {
        px -= camera->getTopX();
        py -= camera->getTopY();
    }
    
    int imgWidth = al_get_bitmap_width(image);
    
    int framesPerRow = imgWidth / frameWidth;
    
    int x = frame % framesPerRow;
    int y = frame / framesPerRow;
    
    al_draw_bitmap_region(image, x * frameWidth, y * frameHeight, frameWidth, frameHeight, (int)px, (int)py, 0);
}

void Spritesheet::setFrame(int fr) {
    if (fr >= 0 && fr < numFrames) {
        frame = fr;
    }
}

int Spritesheet::getFrame() { return frame; }

