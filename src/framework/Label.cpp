
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

#include <vector>
#include <sstream>

#include "Label.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

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
    
    std::vector<std::string>::iterator it;        
    for (it = lines.begin(); it < lines.end(); ++it) {
        std::string line = (std::string) *it;
        float r, g, b;
        al_unmap_rgb_f(color, &r, &g, &b);
        al_draw_text(font, al_map_rgba_f(r, g, b, opacity), px, py, 0, line.c_str());
        py += al_get_font_line_height(font);
    }
}

void Label::setText(const char *txt) {
    
    lines.clear();
    
    text = txt;
    
    int h = 0;
    
    if (wrapText) {
                        
        std::vector<std::string> words;
        split(text, ' ', words);
        
        std::string wrappedText;
        
        int wc = 0;
        
        std::vector<std::string>::iterator it;        
		for (it = words.begin(); it < words.end(); ++it) {
            
            std::string word = (std::string) *it;
            
            std::string eval;
            
            if (wc == 0) {
                eval = word;
            } else {
                eval = wrappedText;
                eval.append(" ").append(word);
            }
            
			int w = al_get_text_width(font, eval.c_str());
            
			// See if the text so far plus the new word fits the rect
			if (w > size.width) {
                
				// If not, closes this line and starts a new one
                lines.push_back(wrappedText);
                wrappedText = word;
                h += al_get_font_line_height(font);
                
			} else {
                if (wc > 0) {
                    wrappedText.append(" ");
                }
                wrappedText.append(word);
			}
            
            wc++;
		}
        
        lines.push_back(wrappedText);
        h += al_get_font_line_height(font);
        
        size = sizeMake(size.width, h);
        
    } else {
        
        int tw = al_get_text_width(font, text.c_str());
        int th = al_get_font_line_height(font);
        
        size = sizeMake(tw, th);
        
        lines.push_back(text);
    }
}

const char *Label::getText() {
    return text.c_str();
}