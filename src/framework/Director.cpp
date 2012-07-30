
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

#include "allegro5/allegro_native_dialog.h"

Director *Director::instance = NULL;

Director *Director::getInstance() {
    
    if (!instance) instance = new Director();
    return instance;
}

void Director::initialize() {
    
    if (!al_init()) {
        abortWithMessage("Failed to init Allegro.\n");
    }
    
    if (!al_init_image_addon()) {
        abortWithMessage("Failed to init image add-on.\n");
    }  
    
    if (!al_init_primitives_addon()) {
        abortWithMessage("Failed to init primitives add-on.\n");
    }
    
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        abortWithMessage("Failed to init TTF add-on.\n");
    }
    
    if (!al_init_acodec_addon()) {
        abortWithMessage("Failed to init audio codec add-on.\n");
    }
    
    if (!al_install_mouse()) {
        abortWithMessage("Failed to init mouse.\n");
    }
    
    if (!al_install_keyboard()) {
        abortWithMessage("Failed to init keyboard.\n");
    }
    
    display = al_create_display(800,600);
    if (!display) {
        abortWithMessage("Failed to create display.\n");
    }
    
    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        abortWithMessage("Failed to create event queue.\n");
    }
    
    displayTimer = al_create_timer(1.0 / 60);
    if (!displayTimer) {
        abortWithMessage("Failed to create display timer.\n");
    }
    
    if (!al_install_audio()) {
        abortWithMessage("Failed to init audio.\n");
    }
    
    if (!al_reserve_samples(10)) {
        abortWithMessage("Failed to reserve samples.\n");
    }
    
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(displayTimer));
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
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
            
            currentScene->handleEvent(ev);
            
            if (ev.type == ALLEGRO_EVENT_TIMER) {
                
                double dt = al_get_time() - lastTime;
                lastTime = al_get_time();
                
                running = currentScene->tick(dt); 
                
                redraw = true; 
                
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                
                running = false;
                nextScene = NULL;
                
            } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                
                currentScene->onKeyDown(ev.keyboard.keycode, ev);
                
            } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                
                currentScene->onKeyUp(ev.keyboard.keycode, ev);
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

ALLEGRO_DISPLAY *Director::getDisplay() {
    return display;
}

void Director::abortWithMessage(const char *format, ...) {
    
    char str[1024];
    va_list args;
    ALLEGRO_DISPLAY *curDisplay;
    
    va_start(args, format);
    vsnprintf(str, sizeof str, format, args);
    va_end(args);
    
    curDisplay = al_is_system_installed() ? al_get_current_display() : NULL;
    al_show_native_message_box(curDisplay, "Error", "Initialization error", str, NULL, 0);
    exit(1);
}
