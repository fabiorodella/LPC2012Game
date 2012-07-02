
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

#include "Scene.h"

Scene::Scene() {
    deleteDisplayList = true;
}

Scene::~Scene() {
    
    if (deleteDisplayList) {
        
        std::vector<Drawable *>::iterator it;
        
        for(it = displayList.begin(); it < displayList.end(); ++it) {
            
            Drawable *drawable = (Drawable *) *it;
            delete drawable;
        }
        
        displayList.clear();
    }
}

bool Scene::tick(double dt) {
    return true;
}

void Scene::draw() {
    
    std::vector<Drawable *>::iterator it;
    
    for(it = displayList.begin(); it < displayList.end(); ++it) {
        
        Drawable *drawable = (Drawable *) *it;
        drawable->draw();
    }
}

void Scene::addToDisplayList(Drawable *drawable) {
    displayList.push_back(drawable);
}

void Scene::removeFromDisplayList(Drawable *drawable) {
    
}

void Scene::removeFromDisplayList(int tag) {
    
}
