
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

#include "ModalDialog.h"

ModalDialog::ModalDialog(const char *text, ALLEGRO_FONT *fnt, const char *confirmMsg, const char *cancelMsg) {
    
    background = new Spritesheet("res/bkg_dialog.png");
    
    label = new Label(text, fnt, al_map_rgb(0, 0, 0), 350);
    
    confirmButton = new Button(confirmMsg, fnt, "res/btn_med.png", "res/btn_med_pressed.png");
    
    cancelButton = NULL;
    
    if (cancelMsg != NULL) {
        cancelButton = new Button(cancelMsg, fnt, "res/btn_med.png", "res/btn_med_pressed.png");
    }
}

void ModalDialog::showInScene(Scene *scene, int zOrder) {
    
    background->setZOrder(zOrder);
    background->setAnchorPoint(pointMake(0.5, 0.5));
    background->setPosition(pointMake(400, 300));
    
    scene->addToDisplayList(background);
    
    label->setZOrder(zOrder + 1);
    label->setAnchorPoint(pointMake(0.5, 0.5));
    label->setPosition(pointMake(400, 270));
    
    scene->addToDisplayList(label);
    
    confirmButton->setZOrder(zOrder + 1);
    confirmButton->setAnchorPoint(pointMake(0.5, 0.5));
    confirmButton->setPosition(pointMake(500, 350));
    confirmButton->setHandler(this);
    
    scene->addToDisplayList(confirmButton);
    
    if (cancelButton != NULL) {
        
        cancelButton->setZOrder(zOrder + 1);
        cancelButton->setAnchorPoint(pointMake(0.5, 0.5));
        cancelButton->setPosition(pointMake(300, 350));
        cancelButton->setHandler(this);
        
        scene->addToDisplayList(cancelButton);
    }
}

void ModalDialog::removeFromScene(Scene *scene) {
    
    scene->removeFromDisplayList(background, true);
    scene->removeFromDisplayList(label, true);
    scene->removeFromDisplayList(confirmButton, true);
    
    if (cancelButton != NULL) {
        scene->removeFromDisplayList(cancelButton, true);
    }
}

void ModalDialog::setHandler(ModalDialogHandler *hnd) {
    handler = hnd;
}

void ModalDialog::onButtonClicked(Button *sender) {
    
    if (handler != NULL) {
        
        if (sender == confirmButton) {
            handler->onConfirm(this);
        } else {
            handler->onCancel(this);
        }
    }
}