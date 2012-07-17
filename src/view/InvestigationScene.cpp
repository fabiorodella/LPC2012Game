
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

#include "InvestigationScene.h"

InvestigationScene::~InvestigationScene() {
    delete mystery;
}

void InvestigationScene::setupScene() {
    
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
            
    playerSprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
    playerSprite->setTag(666);
    playerSprite->setCamera(camera);
    playerSprite->setPosition(pointMake(35 * 32, 50 * 32));
    playerSprite->setAnchorPoint(pointMake(0.5, 0.9));
    playerSprite->setAutoZOrder(true);
    addToDisplayList(playerSprite);
    
    mystery = new Mystery("res/mansion.xml", (unsigned int) time(0), collision->getData(), collision->getSize().width, collision->getSize().height);
    
    long mysteryTime = 0;
    while (!mystery->ended) {
        mystery->step();
        mysteryTime++;
    }
    
    int hr = mysteryTime / (60 * 60);
	int min = (((int)mysteryTime / 60) % 60);
	int sec = (int)mysteryTime % 60; 
    
    printf("Total duration: %ld %.2d:%.2d:%.2d", mysteryTime, hr, min, sec);
    
    std::vector<Character *> characters = mystery->getCharacters();
    std::vector<Character *>::iterator itChars;
    
    int tag = 1;
    
    for (itChars = characters.begin(); itChars < characters.end(); ++itChars) {
        
        Character *character = (Character *) *itChars;
        
        Spritesheet *sprite = new Spritesheet("res/male_walkcycle.png", 64, 64);
        sprite->setTag(tag);
        sprite->setCamera(camera);
        sprite->setAnchorPoint(pointMake(0.5, 0.9));
        sprite->setAutoZOrder(true);
        
        Rect tileRect = collision->getTileRect(character->position.x, character->position.y);
        sprite->setPosition(rectMidPoint(tileRect));
        
        addToDisplayList(sprite);        
    }
    
    camera->setCenter(playerSprite->getPosition());
    
    moving = pointMake(0, 0);
    moveDir = 0;
    curFrame = 0;
    
    escapePressed = false;
    debug = true;
}

bool InvestigationScene::tick(double dt) {
    
    float dx = 0;
    float dy = 0;
    
    bool isMoving = !pointEqualsIntegral(moving, pointMake(0, 0));
    
    if (isMoving) {
        
        dx = moving.x * 200 * dt;
        dy = moving.y * 200 * dt;
        
        if (dy < 0) {
            moveDir = 0;
        } else if (dy > 0) {
            moveDir = 18;
        } else if (dx < 0) {
            moveDir = 9;
        } else if (dx > 0) {
            moveDir = 27;
        }
        
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
    
    return !escapePressed;
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

void InvestigationScene::onKeyDown(int keycode, ALLEGRO_EVENT ev) {
    
    switch (keycode) {
        case ALLEGRO_KEY_UP:
            moving = pointOffset(moving, 0, -1);
            break;
        case ALLEGRO_KEY_DOWN:
            moving = pointOffset(moving, 0, 1);
            break;
        case ALLEGRO_KEY_LEFT:
            moving = pointOffset(moving, -1, 0);
            break;
        case ALLEGRO_KEY_RIGHT:
            moving = pointOffset(moving, 1, 0);
            break;
        default:
            break;
    }
}

void InvestigationScene::onKeyUp(int keycode, ALLEGRO_EVENT ev) {
    
    switch (keycode) {
        case ALLEGRO_KEY_UP:
            moving = pointOffset(moving, 0, 1);
            break;
        case ALLEGRO_KEY_DOWN:
            moving = pointOffset(moving, 0, -1);
            break;
        case ALLEGRO_KEY_LEFT:
            moving = pointOffset(moving, 1, 0);
            break;
        case ALLEGRO_KEY_RIGHT:
            moving = pointOffset(moving, -1, 0);
            break;
        case ALLEGRO_KEY_ESCAPE:
            escapePressed = true;
            break;
        default:
            break;
    }
}
