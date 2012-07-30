
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

#include "BitmapLoader.h"
#include "Director.h"

BitmapLoader *BitmapLoader::instance = NULL;

BitmapLoader *BitmapLoader::getInstance() {
    
    if (!instance) instance = new BitmapLoader();
        return instance;
}

ALLEGRO_BITMAP *BitmapLoader::getBitmap(std::string file) {
    
    Bitmap *ret = bitmaps[file];
            
    if (ret == NULL) {
        
        ALLEGRO_BITMAP *bmpRet = al_load_bitmap(file.c_str());
        if (!bmpRet) {
            Director::getInstance()->abortWithMessage("%s not found or failed to load\n", file.c_str());
        }
        
        ret = new Bitmap();
        ret->bitmap = bmpRet;
        ret->refCount = 1;
        
        bitmaps[file] = ret;
        
    } else {
        
        ret->refCount++;
    }
    
    return ret->bitmap;
}

void BitmapLoader::releaseBitmap(ALLEGRO_BITMAP *bitmap) {
    
    std::map<std::string, Bitmap *>::iterator it = bitmaps.begin();
    std::map<std::string, Bitmap *>::iterator end = bitmaps.end();
    
    while (it != end) {
        
        Bitmap *storedBitmap = (*it).second;
        
        bool deleted = false;
        
        if (storedBitmap->bitmap == bitmap) {
            
            storedBitmap->refCount--;
            
            if (storedBitmap->refCount <= 0) {
                
                bitmaps.erase(it++);
                delete storedBitmap;
                deleted = true;
            }
        }

        if (!deleted) {
            ++it;
        }        
    }
}