
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

#include "Camera.h"

Camera::Camera(int vw, int vh, int bw, int bh) {
    
    viewportSize = SIZE_MAKE(vw, vh);
    
    boundsSize = SIZE_MAKE(bw, bh);
    
    center = POINT_MAKE(0, 0);
}

Size Camera::getViewportSize() {
    
    return viewportSize;
}

Point Camera::getCenter() {
    
    return center;
}

void Camera::setCenter(Point c) {
    
    float cx = c.x;
    float cy = c.y;
    
    float centerX = center.x;
    float centerY = center.y;
    
    if ((cx + (viewportSize.width / 2)) > boundsSize.width) {
        centerX = boundsSize.width - (viewportSize.width / 2);
    } else if ((cx - (viewportSize.width / 2)) < 0) {
        centerX = viewportSize.width / 2;
    } else {
        centerX = cx;
    }
    
    if ((cy + (viewportSize.height / 2)) > boundsSize.height) {
        centerY = boundsSize.height - (viewportSize.height / 2);
    } else if ((cy - (viewportSize.height / 2)) < 0) {
        centerY = viewportSize.height / 2;
    } else {
        centerY = cy;
    }
    
    center = POINT_MAKE(centerX, centerY);
}

Point Camera::getTop() {
    
    float cx = center.x - (viewportSize.width / 2);
    float cy = center.y - (viewportSize.height / 2);
    
    return POINT_MAKE(cx, cy);
}
