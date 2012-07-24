
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

#ifndef LPC2012Game_CaseReportScene_h
#define LPC2012Game_CaseReportScene_h

#include "Framework.h"

class CaseReportScene : public Scene {
    
    unsigned int caseId;
    
    long investigationTime;
    
    int questionsAsked;
    
    int accusationsMade;
    
    bool weaponFound;
    
    bool murdererFound;
    
    ALLEGRO_FONT *font;
    ALLEGRO_FONT *fontBig;
    
    bool endScene;
    
public:
    
    CaseReportScene(unsigned int cId, long iTime, int qAsked, int acMade, bool wFound, bool mFound);
    ~CaseReportScene();
    
    virtual void setupScene();
    virtual bool tick(double dt);
    
    virtual void onKeyUp(int keycode, ALLEGRO_EVENT ev);
};

#endif
