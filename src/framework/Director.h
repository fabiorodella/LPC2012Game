
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

#ifndef LPC2012Game_Director_h
#define LPC2012Game_Director_h

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"

#include "Scene.h"

class Director {
    
private:
    
    Scene *currentScene;
    Scene *nextScene;
    
    ALLEGRO_TIMER *displayTimer;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    
    double lastTime;
    
    static Director *instance;
    
    void displayTick();
    
    Director(){};
    Director(Director const&){};
    
public:
    
    static Director *getInstance();
  
    void initialize();
    
    void startWithScene(Scene *scene);
    void enqueueScene(Scene *scene);
};

#endif
