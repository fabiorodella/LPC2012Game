
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

#ifndef LPC2012Game_Drawable_h
#define LPC2012Game_Drawable_h

#include "Camera.h"

#define Z_ORDER_AUTO_START 2

class Drawable {
    
protected:    

    int tag;
    
    Point position;
    
    Point anchorPoint;
        
    Camera *camera;
    
    int zOrder;
    bool autoZOrder;
    
public:
    
    Drawable();
    virtual ~Drawable() {}
    
    virtual void draw() = 0;
    
    void setTag(int tag);
    int getTag();
    
    void setCamera(Camera *cam);
    Camera *getCamera();
    
    void setPosition(Point p);
    Point getPosition();
    
    void setAnchorPoint(Point p);
    Point getAnchorPoint();
            
    void setZOrder(int z);
    int getZOrder();
    
    void setAutoZOrder(bool z);
    bool isAutoZOrder();
};

#endif
