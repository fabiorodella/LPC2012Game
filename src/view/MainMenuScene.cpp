
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

#include "MainMenuScene.h"
#include "InvestigationScene.h"
#include "Defines.h"

MainMenuScene::~MainMenuScene() {
    
    al_destroy_font(font);
    al_destroy_sample(clickSound);
}

void MainMenuScene::setupScene() {
    
    font = al_load_font("res/AveriaSerif-Regular.ttf", 24, 0);
    if (!font) {
        Director::getInstance()->abortWithMessage("%s not found or failed to load\n", "res/AveriaSerif-Regular.ttf");
    }
    
    clickSound = al_load_sample("res/click.wav");
    if (!clickSound) {
        Director::getInstance()->abortWithMessage("%s not found or failed to load\n", "res/click.wav");
    }
    
    addToDisplayList(new Spritesheet("res/title.png"));
    
    newCaseButton = new Button("New case", font, BTN_TXT_COLOR, "res/btn_action.png", "res/btn_action_pressed.png");
    newCaseButton->setAnchorPoint(pointMake(0.5, 0.5));
    newCaseButton->setPosition(pointMake(400, 500));
    newCaseButton->setHandler(this);
    
    addToDisplayList(newCaseButton);
    
    quitButton = new Button("Quit game", font, BTN_TXT_COLOR, "res/btn_action.png", "res/btn_action_pressed.png");
    quitButton->setAnchorPoint(pointMake(0.5, 0.5));
    quitButton->setPosition(pointMake(400, 550));
    quitButton->setHandler(this);
    
    addToDisplayList(quitButton);
    
    optionSelected = false;
}

bool MainMenuScene::tick(double dt) {
    return !optionSelected;
}

void MainMenuScene::onButtonClicked(Button *sender) {
    
    al_play_sample(clickSound, 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    
    if (sender == newCaseButton) {
        InvestigationScene *scene = new InvestigationScene();
        Director::getInstance()->enqueueScene(scene);
        
        newCaseButton->setLabelText("Generating case...");
        newCaseButton->setEnabled(false);
        
        quitButton->setVisible(false);
        quitButton->setEnabled(false);
        
    }
    
    optionSelected = true;
}