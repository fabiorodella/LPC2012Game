
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

#ifndef LPC2012Game_Scene_h
#define LPC2012Game_Scene_h

#include <vector>

#include "Drawable.h"

class Scene {
    
    std::vector<Drawable *> displayList;
    
    bool deleteDisplayList;
    
public:
    
    Scene();
    virtual ~Scene();
    
    virtual void setupScene(){};
    virtual bool tick(double dt);
    virtual void draw();
    virtual void onKeyDown(int keycode, ALLEGRO_EVENT ev){};
    virtual void onKeyUp(int keycode, ALLEGRO_EVENT ev){};
    virtual void handleEvent(ALLEGRO_EVENT ev);
    
    void addToDisplayList(Drawable *drawable);
    void removeFromDisplayList(Drawable *drawable, bool alsoDelete);
    void removeFromDisplayList(int tag, bool alsoDelete);
    Drawable *getByTag(int tag);
};

#endif
