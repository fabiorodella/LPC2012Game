
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

#include "Director.h"

Director *Director::instance = NULL;

Director *Director::getInstance() {
    
    if (!instance) instance = new Director();
    return instance;
}

void Director::initialize() {
    
    ALLEGRO_DISPLAY *display = NULL;
    
    al_init();    
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    
    display = al_create_display(800,600);
    
    eventQueue = al_create_event_queue();
    
    displayTimer = al_create_timer(1.0 / 60);
    
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(displayTimer));
}

void Director::startWithScene(Scene *scene) {
    
    currentScene = NULL;
    nextScene = scene;
    
    do {
        
        currentScene = nextScene;
        currentScene->setupScene();
        
        nextScene = NULL;
        
        bool running = true;
        bool redraw = true;
        
        lastTime = al_get_time();
        al_start_timer(displayTimer);
        
        while (running) {
            
            ALLEGRO_EVENT ev;
            
            al_wait_for_event(eventQueue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_TIMER) {
                
                double dt = al_get_time() - lastTime;
                lastTime = al_get_time();
                
                running = currentScene->tick(dt); 
                
                redraw = true; 
                
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                
                running = false;
                nextScene = NULL;
            }
            
            if (redraw && al_is_event_queue_empty(eventQueue)) {
                
                redraw = false;
                currentScene->draw();
                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
        
        al_stop_timer(displayTimer);
        
        delete currentScene;
        
    } while (nextScene != NULL);
}

void Director::enqueueScene(Scene *scene) {
    
    delete nextScene;
    nextScene = scene;
}

