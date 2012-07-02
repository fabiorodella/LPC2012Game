
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

#include "TestScene.h"
#include "TestScene2.h"

void TestScene::setupScene() {
    
    std::vector<TilemapLayer *> layers = TilemapLayer::parseTMXFile("res/tilemap.tmx");
    std::vector<TilemapLayer *>::iterator it;
    
    TilemapLayer *firstLayer = layers[0];
    
    camera = new Camera(800, 600, firstLayer->getBoundsWidth(), firstLayer->getBoundsHeight());
    
    for (it = layers.begin(); it < layers.end(); ++it) {
        
        TilemapLayer *layer = (TilemapLayer *) *it;
        layer->setCamera(camera);
        addToDisplayList(layer);
    }
            
    playerSprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
    playerSprite->setTag(1);
    playerSprite->setCamera(camera);    
    playerSprite->setAutoZOrder(true);
    addToDisplayList(playerSprite);
    
    Spritesheet *sprite2 = new Spritesheet("res/male_walkcycle.png", 64, 64);
    sprite2->setTag(2);
    sprite2->setCamera(camera);
    sprite2->setPosX(100);
    sprite2->setAutoZOrder(true);
    addToDisplayList(sprite2);
    
    Spritesheet *sprite3 = new Spritesheet("res/male_walkcycle.png", 64, 64);
    sprite2->setTag(3);
    sprite3->setCamera(camera);
    sprite3->setPosX(200);
    sprite3->setAutoZOrder(true);
    addToDisplayList(sprite3);
    
    camera->setCenterX(playerSprite->getPosX());
    camera->setCenterY(playerSprite->getPosY());
    
    moveDir = 0;
    curFrame = 0;
}

bool TestScene::tick(double dt) {
            
    ALLEGRO_KEYBOARD_STATE kbdstate;
    
    float dx = 0;
    float dy = 0;
    
    bool moving = false;
    
    al_get_keyboard_state(&kbdstate);
    if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE)) {
        
        //Uncomment to test scene flow
        //TestScene2 *next = new TestScene2();
        //Director::getInstance()->enqueueScene(next);
        return false;
    }
    
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
    
    playerSprite->setFrame(moveDir + (int)curFrame);
    playerSprite->setPosX(playerSprite->getPosX() + dx);
    playerSprite->setPosY(playerSprite->getPosY() + dy);
    
    camera->setCenterX(playerSprite->getPosX());
    camera->setCenterY(playerSprite->getPosY());
    
    return true;
}