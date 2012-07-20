
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

MainMenuScene::~MainMenuScene() {
    
    al_destroy_font(font);
}

void MainMenuScene::setupScene() {
    
    font = al_load_font("res/DejaVuSans.ttf", 24, 0);
    
    addToDisplayList(new Spritesheet("res/title.png"));
    
    newCaseButton = new Button("New case", font, "res/btn_action.png", "res/btn_action_pressed.png");
    newCaseButton->setAnchorPoint(pointMake(0.5, 0.5));
    newCaseButton->setPosition(pointMake(400, 300));
    newCaseButton->setHandler(this);
    
    addToDisplayList(newCaseButton);
    
    quitButton = new Button("Quit game", font, "res/btn_action.png", "res/btn_action_pressed.png");
    quitButton->setAnchorPoint(pointMake(0.5, 0.5));
    quitButton->setPosition(pointMake(400, 400));
    quitButton->setHandler(this);
    
    addToDisplayList(quitButton);
    
    generatingLabel = new Label("Generating case...", font, al_map_rgb(0, 0, 0));
    generatingLabel->setAnchorPoint(pointMake(0.5, 0.5));
    generatingLabel->setPosition(pointMake(400, 300));
    generatingLabel->setVisible(false);
    
    addToDisplayList(generatingLabel);
    
    optionSelected = false;
}

bool MainMenuScene::tick(double dt) {
    return !optionSelected;
}

void MainMenuScene::onButtonClicked(Button *sender) {
    
    if (sender == newCaseButton) {
        InvestigationScene *scene = new InvestigationScene();
        Director::getInstance()->enqueueScene(scene);
        
        newCaseButton->setVisible(false);
        newCaseButton->setEnabled(false);
        
        quitButton->setVisible(false);
        quitButton->setEnabled(false);
        
        generatingLabel->setVisible(true);
    }
    
    optionSelected = true;
}