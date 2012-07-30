
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

#include "CaseReportScene.h"
#include "MainMenuScene.h"
#include "Utils.h"

CaseReportScene::CaseReportScene(unsigned int cId, long iTime, int qAsked, int acMade, bool wFound, bool mFound) {
    
    caseId = cId;
    investigationTime = iTime;
    questionsAsked = qAsked;
    accusationsMade = acMade;
    weaponFound = wFound;
    murdererFound = mFound;
    
    endScene = false;
    
}

CaseReportScene::~CaseReportScene() {
    
    al_destroy_font(font);
    al_destroy_font(fontBig);
}

void CaseReportScene::setupScene() {
    
    const char *fontFile = "res/gtw.ttf";
    
    font = al_load_font(fontFile, 30, 0);
    if (!font) {
        Director::getInstance()->abortWithMessage("%s not found or failed to load\n", fontFile);
    }
    
    fontBig = al_load_font(fontFile, 50, 0);
    if (!fontBig) {
        Director::getInstance()->abortWithMessage("%s not found or failed to load\n", fontFile);
    }
    
    int y = 40;
    
    char buff[200];
    
    sprintf(buff, "Case #%d - Report", caseId);
    
    Label *label = new Label(buff, font, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
    
    y += 40;
    
    sprintf(buff, "Total investigation time: %s", timeToString(investigationTime, true).c_str());
    
    label = new Label(buff, font, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
    
    y += 40;
    
    sprintf(buff, "Questions asked: %d", questionsAsked);
    
    label = new Label(buff, font, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
    
    y += 40;
    
    sprintf(buff, "Accusations made: %d", accusationsMade);
    
    label = new Label(buff, font, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
    
    y += 40;
    
    sprintf(buff, "Found murder weapon: %s", weaponFound ? "YES" : "NO");
    
    label = new Label(buff, font, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
    
    y += 80;
    
    sprintf(buff, "CASE %s", murdererFound ? "SOLVED" : "UNSOLVED");
    
    label = new Label(buff, fontBig, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
    
    y += 80;
    
    label = new Label("(press any key to continue)", font, al_map_rgb(255,255,255));
    label->setAnchorPoint(pointMake(0.5, 0));
    label->setPosition(pointMake(400, y));
    
    addToDisplayList(label);
}

bool CaseReportScene::tick(double dt) {
    
    return !endScene;
}

void CaseReportScene::onKeyUp(int keycode, ALLEGRO_EVENT ev) {
    
    MainMenuScene *scene = new MainMenuScene();
    Director::getInstance()->enqueueScene(scene);
    
    endScene = true;
}
