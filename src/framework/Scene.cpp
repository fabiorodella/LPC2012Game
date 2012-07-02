
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

#include <algorithm>

#include "Scene.h"

bool AutoZOrderSortPredicate(Drawable *d1, Drawable *d2) {
    return d1->getPosY() < d2->getPosY();
}

bool ZOrderSortPredicate(Drawable *d1, Drawable *d2) {
    return d1->getZOrder() < d2->getZOrder();
}


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
    
    // This whole process can be VERY optimized if needed
    
    std::vector<Drawable *> autoSort;
    std::vector<Drawable *>::iterator it;
    
    // Gather all auto-sort Drawables in a vector
    for(it = displayList.begin(); it < displayList.end(); ++it) {
        
        Drawable *drawable = (Drawable *) *it;
        
        if (drawable->isAutoZOrder()) {
            autoSort.push_back(drawable);
        }
    }
    
    // Sort this vector by Y position
    std::sort(autoSort.begin(), autoSort.end(), AutoZOrderSortPredicate);
    
    // Assigns z-order values to elements, starting with Z_ORDER_AUTO_START
    int z = Z_ORDER_AUTO_START;
    
    for(it = autoSort.begin(); it < autoSort.end(); ++it) {
        
        Drawable *drawable = (Drawable *) *it;
        drawable->setZOrder(z);
        z++;
    }
    
    // Sorts the display list based on the z-order values
    std::sort(displayList.begin(), displayList.end(), ZOrderSortPredicate);
    
    // Draws everything in the correct order
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
