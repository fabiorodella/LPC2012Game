
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

#include "Drawable.h"

Drawable::Drawable() {
    
    tag = 0;
    camera = 0;
    posX = 0;
    posY = 0;
    anchorX = 0;
    anchorY = 0;
    zOrder = 0;
    autoZOrder = false;
}

void Drawable::setTag(int t) {
    tag = t;
}

int Drawable::getTag() {
    return tag;
}

void Drawable::setCamera(Camera *cam) {
    camera = cam;
}

Camera *Drawable::getCamera() {
    return camera;
}

void Drawable::setPosX(float px) {
    posX = px;
}

float Drawable::getPosX() {
    return posX;
}

void Drawable::setPosY(float py) {
    posY = py;
}

float Drawable::getPosY() {
    return posY;
}

void Drawable::setAnchorX(float px) {
    anchorX = px;
}

float Drawable::getAnchorX() {
    return anchorX;
}

void Drawable::setAnchorY(float py) {
    anchorY = py;
}

float Drawable::getAnchorY() {
    return anchorY;
}

void Drawable::setZOrder(int z) {
    zOrder = z;
}

int Drawable::getZOrder() {
    return zOrder;
}

void Drawable::setAutoZOrder(bool z) {
    autoZOrder = z;
}

bool Drawable::isAutoZOrder() {
    return autoZOrder;
}
