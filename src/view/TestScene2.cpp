//
//  TestScene2.cpp
//  LPC2012Game
//
//  Created by Fábio Rodella on 7/2/12.
//  Copyright (c) 2012 Crocodella. All rights reserved.
//

#include "TestScene2.h"

void TestScene2::setupScene() {
    
    std::vector<TilemapLayer *> layers = TilemapLayer::parseTMXFile("res/tilemap.tmx");
    std::vector<TilemapLayer *>::iterator it;
    
    TilemapLayer *firstLayer = layers[0];
    
    camera = new Camera(800, 600, firstLayer->getBoundsWidth(), firstLayer->getBoundsHeight());
    
    for (it = layers.begin(); it < layers.end(); ++it) {
        
        TilemapLayer *layer = (TilemapLayer *) *it;
        layer->setCamera(camera);
        addToDisplayList(layer);
    }
}

bool TestScene2::tick(double dt) {
    
    ALLEGRO_KEYBOARD_STATE kbdstate;
    
    al_get_keyboard_state(&kbdstate);
    if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE)) {
        return false;
    }
    
    camera->setCenterX(camera->getCenterX() + (20 * dt));
    camera->setCenterY(camera->getCenterY() + (20 * dt));
            
    return true;
}
