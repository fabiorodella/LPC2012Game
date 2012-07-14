
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

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "DebugCrimeScene.h"

DebugCrimeScene::~DebugCrimeScene() {
    delete mystery;
}

void DebugCrimeScene::setupScene() {
    
    font = al_load_font("res/DejaVuSans.ttf", 16, 0);
    
    std::vector<TilemapLayer *> layers = TilemapLayer::parseTMXFile("res/mansion.tmx");
    std::vector<TilemapLayer *>::iterator it;
    
    TilemapLayer *firstLayer = layers[0];
    
    camera = new Camera(800, 600, firstLayer->getBoundsSize().width, firstLayer->getBoundsSize().height);
    
    for (it = layers.begin(); it < layers.end(); ++it) {
        
        TilemapLayer *layer = (TilemapLayer *) *it;
        layer->setCamera(camera);
        addToDisplayList(layer);
        
        if (layer->isCollision()) {
            collision = layer;
        }
    }
    
    mystery = new Mystery("res/mansion.xml", (unsigned int) time(0), collision->getData(), collision->getSize().width, collision->getSize().height);
    
    std::vector<Character *> characters = mystery->getCharacters();
    std::vector<Character *>::iterator itChars;
    
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        
        Character *character = (Character *) *itChars;
        
        Spritesheet *sprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
        sprite->setTag(character->tag);
        sprite->setCamera(camera);
        sprite->setAnchorPoint(pointMake(0.5, 0.9));
        sprite->setAutoZOrder(true);
        
        Rect tileRect = collision->getTileRect(character->position.x, character->position.y);
        sprite->setPosition(rectMidPoint(tileRect));
        
        addToDisplayList(sprite);        
    }
    
    std::vector<POI *>::iterator itWeapons;
    
    int i = 0;
    
    for (itWeapons = mystery->weapons.begin(); itWeapons < mystery->weapons.end(); ++itWeapons) {
        
        POI *weapon = (POI *) *itWeapons;
        
        Spritesheet *sprite = new Spritesheet("res/weapons.png", 32, 32);
        sprite->setTag(i);
        sprite->setFrame(i);
        sprite->setCamera(camera);
        sprite->setAnchorPoint(pointMake(0.5, 0.9));
        sprite->setZOrder(400);
        
        Rect tileRect = collision->getTileRect(weapon->position.x, weapon->position.y);
        sprite->setPosition(rectMidPoint(tileRect));
        
        addToDisplayList(sprite);
        
        i++;
    }
    
    camera->setCenter(pointMake(400, 300));
    
    mysteryTime = 0;
    totalTime = 0;
}

bool DebugCrimeScene::tick(double dt) {
    
    ALLEGRO_KEYBOARD_STATE kbdstate;
    
    float dx = 0;
    float dy = 0;
    
    al_get_keyboard_state(&kbdstate);
    if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE)) {
        return false;
    }
    
    if (al_key_down(&kbdstate, ALLEGRO_KEY_UP)) { 
        dy -= 800 * dt;
    }
    
    if (al_key_down(&kbdstate, ALLEGRO_KEY_DOWN)) {
        dy += 800 * dt;
    }
    
    if (al_key_down(&kbdstate, ALLEGRO_KEY_LEFT)) {
        dx -= 800 * dt;
    }
    
    if (al_key_down(&kbdstate, ALLEGRO_KEY_RIGHT)) {
        dx += 800 * dt;
    }
    
    camera->setCenter(pointOffset(camera->getCenter(), dx, dy));
    
    totalTime += 1;
        
    if (totalTime % 3 == 0) {
        
        mystery->step();
        
        mysteryTime++;
                        
        std::vector<Character *> characters = mystery->getCharacters();
        std::vector<Character *>::iterator itChars;
        
        for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
            
            Character *character = (Character *) *itChars;
            
            Drawable *sprite = getByTag(character->tag);
            
            Rect tileRect = collision->getTileRect(character->position.x, character->position.y);
            sprite->setPosition(rectMidPoint(tileRect));
        }
    }
    
    return true;
}

void DebugCrimeScene::draw() {
    
    Scene::draw();
    
    std::vector<Character *> characters = mystery->getCharacters();
    std::vector<Character *>::iterator itChars;
    
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        
        Character *character = (Character *) *itChars;
        
        Drawable *sprite = getByTag(character->tag);
    
        ALLEGRO_COLOR col = al_map_rgb(0, 0, 0);
        if (character->murderTarget != NULL) {
            col = al_map_rgb(255, 0, 0);
        } else if (character == mystery->victim) {
            col = al_map_rgb(255, 255, 255);
        }
        
        al_draw_text(font, col, sprite->getPosition().x - camera->getTop().x,  sprite->getPosition().y - camera->getTop().y, 0, character->name.c_str());
    }
    
    char *txt = new char[50];
    
    int hr = mysteryTime / (60 * 60);
	int min = (((int)mysteryTime / 60) % 60);
	int sec = (int)mysteryTime % 60; 
    
    sprintf(txt, "%ld %.2d:%.2d:%.2d",mysteryTime, hr,min,sec);
    
    al_draw_text(font, al_map_rgb(0, 0, 0), 10,  10, 0, txt);
    
    delete txt;
}
