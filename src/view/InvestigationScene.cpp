
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

#include "allegro5/allegro_primitives.h"

#include "InvestigationScene.h"

InvestigationScene::~InvestigationScene() {
    delete controller;
}

void InvestigationScene::setupScene() {
    
    std::vector<TilemapLayer *> layers = TilemapLayer::parseTMXFile("res/rooms.tmx");
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
    
    controller = new MysteryController();
    controller->generateMystery(1, 10, collision->getData());    
    
    playerSprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
    playerSprite->setTag(1);
    playerSprite->setCamera(camera);
    playerSprite->setPosition(pointMake(100, 100));
    playerSprite->setAnchorPoint(pointMake(0.5, 0.9));
    playerSprite->setAutoZOrder(true);
    addToDisplayList(playerSprite);
    
    camera->setCenter(playerSprite->getPosition());
    
    moveDir = 0;
    curFrame = 0;
    
    debug = true;
}

bool InvestigationScene::tick(double dt) {
    
    ALLEGRO_KEYBOARD_STATE kbdstate;
    
    float dx = 0;
    float dy = 0;
    
    bool moving = false;
    
    al_get_keyboard_state(&kbdstate);
    if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE)) {
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
    
    bool collided = false;
    
    Rect colRect = rectMake(playerSprite->getPosition().x - 16, playerSprite->getPosition().y - 16, 32, 32);
    
    Rect colRectX = rectOffset(colRect, dx, 0);
    Rect colRectY = rectOffset(colRect, 0, dy);
    
    int tw = collision->getTileSize().width;
    int th = collision->getTileSize().height;
    
    int tpx = playerSprite->getPosition().x / collision->getTileSize().width;
    int tpy = playerSprite->getPosition().y / collision->getTileSize().height;
    
    for (int j = tpy - 2; j <= tpy + 2; j++) {
        for (int i = tpx - 2; i <= tpx + 2; i++) {
            
            if (i >= 0 && i < collision->getSize().width && j >= 0 && j < collision->getSize().height) {
                int gid = collision->getTileAt(i, j);
                
                if (gid != 0) {
                    
                    Rect tileRect = rectMake(i * tw, j * th, tw, th);
                    
                    if (rectIntersectsRect(colRectX, tileRect)) {
                        dx = 0;
                        collided = true;
                    }
                    
                    if (rectIntersectsRect(colRectY, tileRect)) {
                        dy = 0;
                        collided = true;
                    }
                }
            }
        }
    }
    
    playerSprite->setFrame(moveDir + (int)curFrame);
    playerSprite->setPosition(pointOffset(playerSprite->getPosition(), dx, dy));
    
    camera->setCenter(playerSprite->getPosition());
    
    return true;
}

void InvestigationScene::draw() {
    Scene::draw();
    
    if (debug) {
        
        al_draw_rectangle(playerSprite->getPosition().x - 16 - camera->getTop().x, playerSprite->getPosition().y - 16 - camera->getTop().y, playerSprite->getPosition().x + 16 - camera->getTop().x, playerSprite->getPosition().y + 16 - camera->getTop().y, al_map_rgb(255, 0, 0), 1);
        
        int tw = collision->getTileSize().width;
        int th = collision->getTileSize().height;
        
        int tpx = playerSprite->getPosition().x / collision->getTileSize().width;
        int tpy = playerSprite->getPosition().y / collision->getTileSize().height;
        
        for (int j = tpy - 2; j <= tpy + 2; j++) {
            for (int i = tpx - 2; i <= tpx + 2; i++) {
                
                if (i >= 0 && i < collision->getSize().width && j >= 0 && j < collision->getSize().height) {
                    int gid = collision->getTileAt(i, j);
                    
                    if (gid != 0) {
                        
                        al_draw_rectangle(i * tw - camera->getTop().x, j * th - camera->getTop().y, i * tw + tw - camera->getTop().x, j * th + th - camera->getTop().y, al_map_rgb(255, 0, 0), 1);
                    }
                }
            }
        }
    }
}
