//
//  TestScene2.h
//  LPC2012Game
//
//  Created by Fábio Rodella on 7/2/12.
//  Copyright (c) 2012 Crocodella. All rights reserved.
//

#ifndef LPC2012Game_TestScene2_h
#define LPC2012Game_TestScene2_h

#include "Framework.h"

class TestScene2 : public Scene {
    
    Camera *camera;
    
public:
    
    virtual void setupScene();
    virtual bool tick(double dt);
    
};

#endif
