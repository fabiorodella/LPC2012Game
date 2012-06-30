
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

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

#include "Camera.h"
#include "Spritesheet.h"
#include "Tilemap.h"

int main(int argc, char **argv) {
    
    bool done = false;
    
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_KEYBOARD_STATE kbdstate;
    
    al_init();    
    al_init_image_addon();
    al_install_keyboard();
    
    display = al_create_display(800,600);
    
    Tilemap *tilemap = new Tilemap("res/tilemap.tmx");
    
    Camera *camera = new Camera(800, 600, tilemap->getBoundsWidth(), tilemap->getBoundsHeight());
        
    tilemap->setCamera(camera);
    
    Spritesheet *sprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
    sprite->setCamera(camera);    
    
    double lastTime = al_get_time();    
    
    int moveDir = 0;
    float curFrame = 0;
    
    bool moving = false;
    
    while(!done) {
        
        double dt = al_get_time() - lastTime;
        lastTime = al_get_time();
        
        float dx = 0;
        float dy = 0;
        
        moving = false;
        
        al_get_keyboard_state(&kbdstate);
        if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE)) done = true;
        
        if (al_key_down(&kbdstate, ALLEGRO_KEY_UP)) { 
            dy -= 200 * dt;
            moveDir = 0;
            moving = true;
        }
        
        if (al_key_down(&kbdstate, ALLEGRO_KEY_DOWN)) {
            dy += 200 * dt;
            moveDir = 18;
            moving = true;
        }
        
        if (al_key_down(&kbdstate, ALLEGRO_KEY_LEFT)) {
            dx -= 200 * dt;
            moveDir = 9;
            moving = true;
        }
        
        if (al_key_down(&kbdstate, ALLEGRO_KEY_RIGHT)) {
            dx += 200 * dt;
            moveDir = 27;
            moving = true;
        }
        
        if (moving) {
            curFrame += dt * 10;
            if (curFrame > 9) curFrame -= 9;
        } else {
            curFrame = 0;
        }
        
        sprite->setFrame(moveDir + (int)curFrame);
        sprite->setPosX(sprite->getPosX() + dx);
        sprite->setPosY(sprite->getPosY() + dy);
        
        camera->setCenterX(sprite->getPosX());
        camera->setCenterY(sprite->getPosY());
        
        tilemap->draw();
               
        sprite->draw();
                                        
		al_flip_display();
		al_clear_to_color(al_map_rgb(255,0,0));
	}
                
    delete sprite;
    delete tilemap;
    delete camera;
    
    al_destroy_display(display);
            
    return 0;
}
