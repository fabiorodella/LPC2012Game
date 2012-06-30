
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

#ifndef LPC2012Game_Tilemap_h
#define LPC2012Game_Tilemap_h

#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Spritesheet.h"

class Tilemap : public Drawable {
    
    Spritesheet *tileset;
    
    int tileWidth;
    int tileHeight;
    
    int mapWidth;
    int mapHeight;
    
    int numLayers;
    
    short *data;
    
public:
    
    Tilemap();
    Tilemap(const char *file);
    ~Tilemap();
    
    virtual void draw();
    void drawLayer(int layer);
    
    void setTileAt(short tile, int x, int y, int layer);
    short getTileAt(int x, int y, int layer);
    
    int getBoundsWidth();
    int getBoundsHeight();
    
private:
    
    xmlNode *getFirstChildForName(xmlNode *parent, const char *name);
    
    std::vector<xmlNode *> getChildrenForName(xmlNode *parent, const char *name);
    
    char *getXmlAttribute(xmlNode *node, const char *name);
    
    void decodeLayerData(xmlNode *dataNode, int layerIdx);
};

#endif
