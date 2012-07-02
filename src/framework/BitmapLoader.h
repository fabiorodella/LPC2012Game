
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

#ifndef LPC2012Game_BitmapLoader_h
#define LPC2012Game_BitmapLoader_h

#include <map>

#include "allegro5/allegro.h"

struct Bitmap {
    ALLEGRO_BITMAP *bitmap;
    int refCount;
};

class BitmapLoader {
    
private:

    std::map<const char *, Bitmap *> bitmaps;
    
    static BitmapLoader *instance;

    BitmapLoader(){};
    BitmapLoader(BitmapLoader const&){};
    
public:

    static BitmapLoader *getInstance();
    
    ALLEGRO_BITMAP *getBitmap(const char *file);
    
    void releaseBitmap(ALLEGRO_BITMAP *bitmap);
};

#endif
