
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

#ifndef LPC2012Game_ModalDialog_h
#define LPC2012Game_ModalDialog_h

#include "Spritesheet.h"
#include "Label.h"
#include "Button.h"
#include "Scene.h"

class ModalDialog;

class ModalDialogHandler {
    
public:
    virtual void onConfirm(ModalDialog *sender) = 0;
    virtual void onCancel(ModalDialog *sender) = 0;
};

class ModalDialog : public ButtonHandler {
    
    Spritesheet *background;
    
    Label *label;
    
    Button *confirmButton;
    
    Button *cancelButton;
    
    ModalDialogHandler *handler;
    
public:
    
    int tag;
    
    ModalDialog(const char *text, ALLEGRO_FONT *fnt, const char *confirmMsg, const char *cancelMsg);
    
    void showInScene(Scene *scene, int zOrder);
    void removeFromScene(Scene *scene);
    
    void setHandler(ModalDialogHandler *hnd);
    
    virtual void onButtonClicked(Button *sender);
};


#endif
