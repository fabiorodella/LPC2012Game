
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

#include "TutorialScene.h"
#include "MainMenuScene.h"

void TutorialScene::setupScene() {
    
    char c = '1';
    
    for (int i = 0; i < 5; i++) {
        
        std::string filename = "res/tutorial";
        filename.push_back(c);
        filename.append(".png");
        
        Spritesheet *sprite = new Spritesheet(filename.c_str());
        sprite->setPosition(pointMake(0, 0));
        sprite->setZOrder(i);
        
        if (i > 0) {
            sprite->setVisible(false);
        }
        
        addToDisplayList(sprite);
        
        c++;
    }
    
    idx = 0;
    endScene = false;
}

bool TutorialScene::tick(double dt) {
    return !endScene;
}

void TutorialScene::handleEvent(ALLEGRO_EVENT ev) {
    
    if (ev.type == ALLEGRO_EVENT_KEY_UP || ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        
        displayList[idx]->setVisible(false);
        
        idx++;
        
        if (idx < displayList.size()) {
            
            displayList[idx]->setVisible(true);
            
        } else {
        
            MainMenuScene *scene = new MainMenuScene();
            Director::getInstance()->enqueueScene(scene);
            
            endScene = true;
        }
    }
}
