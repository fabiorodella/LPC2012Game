
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

#define LAYER_Z_ORDER_PROPERTY "z_order"
#define LAYER_COLLISION_PROPERTY "collision"

class TilemapLayer : public Drawable {
    
    Spritesheet *tileset;
    
    Size tileSize;
    
    Size mapSize;
            
    int numLayers;
    
    short *data;
    
    bool collision;
    
    TilemapLayer(){};
    TilemapLayer(TilemapLayer const&){};
    
    static xmlNode *getFirstChildForName(xmlNode *parent, const char *name);
    
    static std::vector<xmlNode *> getChildrenForName(xmlNode *parent, const char *name);
    
    static char *getXmlAttribute(xmlNode *node, const char *name);
    
    static void decodeLayerData(xmlNode *dataNode, TilemapLayer *layer);
    
public:
    
    ~TilemapLayer();
    
    static std::vector<TilemapLayer *> parseTMXFile(const char *file);
    
    virtual void draw();
    
    void setTileAt(short tile, int x, int y);
    short getTileAt(int x, int y);
    
    Size getSize();
    
    Size getBoundsSize();
    
    Size getTileSize();
    
    short *getData();
    
    bool isCollision();
};

#endif
