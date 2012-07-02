//
//  TestScene.h
//  LPC2012Game
//
//  Created by Fábio Rodella on 7/2/12.
//  Copyright (c) 2012 Crocodella. All rights reserved.
//

#ifndef LPC2012Game_TestScene_h
#define LPC2012Game_TestScene_h

#include "Framework.h"

class TestScene : public Scene {
    
    Spritesheet *playerSprite;
    
    Camera *camera;
    
    int moveDir;
    float curFrame;
    
public:
    
    virtual void setupScene();
    virtual bool tick(double dt);
    
};

#endif
