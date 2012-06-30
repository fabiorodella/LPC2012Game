
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
    
    viewportWidth = vw;
    viewportHeight = vh;
    
    boundsWidth = bw;
    boundsHeight = bh;
    
    centerX = centerY = 0;
}

int Camera::getViewportWidth() {
    return viewportWidth;
}

int Camera::getViewportHeight() {
    return viewportHeight;
}

float Camera::getCenterX() {
    return centerX;
}

void Camera::setCenterX(float cx) {
    if ((cx + (viewportWidth / 2)) > boundsWidth) {
        centerX = boundsWidth - (viewportWidth / 2);
    } else if ((cx - (viewportWidth / 2)) < 0) {
        centerX = viewportWidth / 2;
    } else {
        centerX = cx;
    }
}

float Camera::getCenterY() {
    return centerY;
}

void Camera::setCenterY(float cy) {
    if ((cy + (viewportHeight / 2)) > boundsHeight) {
        centerY = boundsHeight - (viewportHeight / 2);
    } else if ((cy - (viewportHeight / 2)) < 0) {
        centerY = viewportHeight / 2;
    } else {
        centerY = cy;
    }
}

float Camera::getTopX() {
    return centerX - (viewportWidth / 2);
}

float Camera::getTopY() {
    return centerY - (viewportHeight / 2);
}