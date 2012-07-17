
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

#include "Label.h"

Label::Label(const char *txt, ALLEGRO_FONT *fnt, ALLEGRO_COLOR col) {
    
    font = fnt;
    color = col;
    
    wrapText = false;
    
    setText(txt);
}

Label::Label(const char *txt, ALLEGRO_FONT *fnt, ALLEGRO_COLOR col, int width) {
    
    font = fnt;
    color = col;
    
    wrapText = true;
    
    size = sizeMake(width, 0);
    
    setText(txt);
}

void Label::draw() {
    
    float px = position.x - (anchorPoint.x * size.width);
    float py = position.y - (anchorPoint.y * size.height);
    
    if (camera != NULL) {
        px -= camera->getTop().x;
        py -= camera->getTop().y;
    }
    
    al_draw_text(font, color, px, py, 0, text.c_str());
}

void Label::setText(const char *txt) {
    
    text = txt;
        
    if (wrapText) {
        
        processedText.clear();
        
        
        
    } else {
        
        int tw = al_get_text_width(font, text.c_str());
        int th = al_get_font_line_height(font);
        
        size = sizeMake(tw, th);
        
        processedText = text;
    }
}

const char *Label::getText() {
    return text.c_str();
}