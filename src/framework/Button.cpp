
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

#include "Button.h"
#include "BitmapLoader.h"

Button::Button(const char *text, ALLEGRO_FONT *fnt, ALLEGRO_COLOR col, const char *normalImageFile, const char *pressedImageFile) {
    
    labelText = text;
    font = fnt;
    color = col;
    
    normalImage = BitmapLoader::getInstance()->getBitmap(normalImageFile);
    pressedImage = BitmapLoader::getInstance()->getBitmap(pressedImageFile);
    
    size = sizeMake(al_get_bitmap_width(normalImage), al_get_bitmap_height(normalImage));
    
    handler = NULL;
    
    enabled = true;
    pressed = false;
    
    data = NULL;
}

Button::Button(Size sz) {
    
    normalImage = NULL;
    pressedImage = NULL;
    
    size = sz;
    
    handler = NULL;
    
    enabled = true;
    pressed = false;
    
    data = NULL;
    
    visible = false;
}

Button::~Button() {
    
    if (normalImage != NULL) {
        BitmapLoader::getInstance()->releaseBitmap(normalImage);
    }
    
    if (pressedImage != NULL) {
        BitmapLoader::getInstance()->releaseBitmap(pressedImage);
    }
}

void Button::draw() {
    
    float px = position.x - (anchorPoint.x * size.width);
    float py = position.y - (anchorPoint.y * size.height);
    
    if (camera != NULL) {
        px -= camera->getTop().x;
        py -= camera->getTop().y;
    }
    
    ALLEGRO_BITMAP *bmp = pressed ? pressedImage : normalImage;
    
    al_draw_tinted_bitmap(bmp, al_map_rgba_f(1, 1, 1, opacity), (int)px, (int)py, 0);
        
    if (!labelText.empty()) {
        
        int tw = al_get_text_width(font, labelText.c_str());
        int th = al_get_font_line_height(font);
        int tx = px + (size.width / 2) - (tw / 2);
        int ty = py + (size.height / 2) - (th / 2);
        
        if (pressed) {
            ty++;
        }
        
        al_draw_text(font, color, tx, ty, 0, labelText.c_str());
    }
}

void Button::handleEvent(ALLEGRO_EVENT ev) {
    
    if (!enabled) {
        return;
    }
    
    float px = position.x - (anchorPoint.x * size.width);
    float py = position.y - (anchorPoint.y * size.height);
    
    if (camera != NULL) {
        px -= camera->getTop().x;
        py -= camera->getTop().y;
    }
    
    Rect rect = rectMake(px, py, size.width, size.height);
    
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !pressed && ev.mouse.button == 1) {
        
        Point mousePoint = pointMake(ev.mouse.x, ev.mouse.y);
        
        if (rectContainsPoint(rect, mousePoint)) {
            pressed = true;
        }
        
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && pressed && ev.mouse.button == 1) {
        
        pressed = false;
        
        Point mousePoint = pointMake(ev.mouse.x, ev.mouse.y);
        
        if (handler != NULL && rectContainsPoint(rect, mousePoint)) {
            handler->onButtonClicked(this);
        }
    }
}

void Button::setVisible(bool vis) {
    if (normalImage != NULL) {
        visible = vis;
    }
}

void Button::setLabelText(const char *text) {
    labelText = text;
}

const char *Button::getLabelText() {
    return labelText.c_str();
}

void Button::setEnabled(bool en) {
    enabled = en;
}

bool Button::isEnabled() {
    return enabled;
}

void Button::setHandler(ButtonHandler *btnHandler) {
    handler = btnHandler;
}
