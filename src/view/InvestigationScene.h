
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

#ifndef LPC2012Game_InvestigationScene_h
#define LPC2012Game_InvestigationScene_h

#include "Framework.h"

#include "ModalDialog.h"

#include "Mystery.h"

class InvestigationScene : public Scene, ButtonHandler, ModalDialogHandler {
    
    ALLEGRO_FONT *font;
    ALLEGRO_FONT *fontBig;
    
    Mystery *mystery;
    long mysteryTime;
    
    Spritesheet *playerSprite;
    
    Camera *camera;
    
    TilemapLayer *collision;
    
    Label *currentRoomLabel;
    
    Button *actionButton;
    
    Spritesheet *bkgQuestion;
    Label *questionLabel;
    Label *whenLabel;
    std::vector<Drawable *> questionElements;
    Button *askQuestionButton;
    Button *cancelQuestionButton;
    
    Spritesheet *bkgSpeech;
    Label *speechLabel;
    Button *speechButton;
    
    std::vector<std::string> speechLines;
    int speechIdx;
    
    Character *activeCharacter;
    POI *activePOI;
    
    Room *currentRoom;
    
    MemoryFilter currentFilter;
    
    Point moving;
    int moveDir;
    float curFrame;
    
    bool inputLocked;
    bool endScene;
    bool debug;
    
public:
    
    ~InvestigationScene();
    
    virtual void setupScene();
    virtual bool tick(double dt);
    virtual void draw();
    virtual void onKeyDown(int keycode, ALLEGRO_EVENT ev);
    virtual void onKeyUp(int keycode, ALLEGRO_EVENT ev);
    
    virtual void onButtonClicked(Button *sender);
    
    virtual void onConfirm(ModalDialog *sender);
    virtual void onCancel(ModalDialog *sender);
    
    void removeQuestionElements();
    void questionStart();
    void questionWho();
    void questionWhere();
    void questionWhen();
    void questionEnd();
    
    void dialogueStart();
    void dialogueAdvance();
    void dialogueEnd();
    
    void quitToMenu();
    void confirmSuspect();
};

#endif
