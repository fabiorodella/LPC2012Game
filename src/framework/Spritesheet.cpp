
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
#include "BitmapLoader.h"

Spritesheet::Spritesheet(char const *file) {
        
    image = BitmapLoader::getInstance()->getBitmap(file);
    
    numFrames = 1;
    
    frame = 0;
    
    frameSize = sizeMake(al_get_bitmap_width(image), al_get_bitmap_height(image));
}

Spritesheet::Spritesheet(char const *file, int fw, int fh) {
    
    image = BitmapLoader::getInstance()->getBitmap(file);
    
    numFrames = 0;
    
    frame = 0;
    
    frameSize = sizeMake(fw, fh);
    
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
    
    BitmapLoader::getInstance()->releaseBitmap(image);
}

void Spritesheet::draw() {
            
    float px = position.x - (anchorPoint.x * frameSize.width);
    float py = position.y - (anchorPoint.y * frameSize.height);
    
    if (camera != NULL) {
        px -= camera->getTop().x;
        py -= camera->getTop().y;
    }
    
    int imgWidth = al_get_bitmap_width(image);
    
    int framesPerRow = imgWidth / frameSize.width;
    
    int x = frame % framesPerRow;
    int y = frame / framesPerRow;
    
    al_draw_tinted_bitmap_region(image, al_map_rgba_f(1, 1, 1, opacity), x * frameSize.width, y * frameSize.height, frameSize.width, frameSize.height, (int)px, (int)py, 0);
}

void Spritesheet::setFrame(int fr) {
    if (fr >= 0 && fr < numFrames) {
        frame = fr;
    }
}

int Spritesheet::getFrame() {
    return frame;
}

Size Spritesheet::getFrameSize() {
    return frameSize;
}
