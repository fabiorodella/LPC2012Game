
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

#include "Tilemap.h"

Tilemap::Tilemap() {
    
    camera = NULL;
    
    posX = 0;
    posY = 0;
    
    mapWidth = 200;
    mapHeight = 200;
    
    numLayers = 1;
    
    tileWidth = tileHeight = 64;
            
    tileset = new Spritesheet("res/townV2.png", tileWidth, tileHeight);
    
    data = new short[mapWidth * mapHeight * numLayers];
    
    for (int j = 0; j < mapHeight; j++) {
        for (int i = 0; i < mapWidth; i++) {
            data[j * mapWidth + i] = rand() % 4;
        }
    }
}

Tilemap::Tilemap(const char *file) {
    
    camera = NULL;
    
    posX = 0;
    posY = 0;
    
    // THIS IS NOT a complete TMX parser! It will only work with:
    // - orthogonal maps
    // - single tileset
    // - all layers with same size
    // - tile data without compression
    // - no flipped tiles
    // - firstGid = 1
    
    xmlDoc *doc = xmlReadFile(file, NULL, 0);
    
    xmlNode *root = xmlDocGetRootElement(doc);
        
	mapWidth = atoi(getXmlAttribute(root, "width"));
	mapHeight = atoi(getXmlAttribute(root, "height"));
	tileWidth = atoi(getXmlAttribute(root, "tilewidth"));
	tileHeight = atoi(getXmlAttribute(root, "tileheight"));
            
    xmlNode *tilesetNode = getFirstChildForName(root, "tileset");
    xmlNode *imageNode = getFirstChildForName(tilesetNode, "image");
    
    const char *imageFile = getXmlAttribute(imageNode, "source");
    
    tileset = new Spritesheet(imageFile, tileWidth, tileHeight);
    
    std::vector<xmlNode *> layerNodes = getChildrenForName(root, "layer");
    std::vector<xmlNode *>::iterator layerNodesIt;
    
    numLayers = layerNodes.size();
    
    data = new short[mapWidth * mapHeight * numLayers];
    
    int i = 0;    
    
    for(layerNodesIt = layerNodes.begin(); layerNodesIt < layerNodes.end(); ++layerNodesIt) {
        
        xmlNode *layerNode = (xmlNode *) *layerNodesIt;
        decodeLayerData(getFirstChildForName(layerNode, "data"), i);
        i++;
    }    
}

Tilemap::~Tilemap() {
    
    delete data;
    delete tileset;
}

void Tilemap::draw() {
    
    for (int l = 0; l < numLayers; l++) {
        drawLayer(l);
    }
}

void Tilemap::drawLayer(int layer) {
    
    //TODO for now, maps can only be positioned at origin (0,0)
    
    float topX = 0;
    float topY = 0;
    
    int tilesX = 0;
    int tilesY = 0;
    
    if (camera != NULL) {
        
        topX = camera->getTopX();
        topY = camera->getTopY();
        
        tilesX = camera->getViewportWidth() / tileWidth;
        tilesY = camera->getViewportHeight() / tileHeight;
        
    } else {
        
        tilesX = al_get_display_width(al_get_current_display()) / tileWidth;
        tilesY = al_get_display_height(al_get_current_display()) / tileHeight;
    }
    
    int tx = topX / tileWidth;
    int ty = topY / tileHeight;
    
    int ox = -camera->getTopX();
    int oy = -camera->getTopY();
    
    al_hold_bitmap_drawing(true);
    
    for (int j = ty; j < (ty + tilesY + 2); j++) {
        for (int i = tx; i < (tx + tilesX + 2); i++) {
                                    
            if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight && getTileAt(i, j, layer) > 0) {
            
                short tile = getTileAt(i, j, layer) - 1;
                tileset->setFrame(tile);                                               
                tileset->setPosX(i * tileWidth + ox);
                tileset->setPosY(j * tileHeight + oy);
                tileset->draw();
            }
        }
    }
    
    al_hold_bitmap_drawing(false);
}

void Tilemap::setTileAt(short tile, int x, int y, int layer) {
    
    data[(y * mapWidth + x) + (layer * mapWidth * mapHeight)] = tile;
}

short Tilemap::getTileAt(int x, int y, int layer) {
    
    return data[(y * mapWidth + x) + (layer * mapWidth * mapHeight)];
}

int Tilemap::getBoundsWidth() {
    
    return mapWidth * tileWidth;
}

int Tilemap::getBoundsHeight() {
    
    return mapHeight * tileHeight;
}

xmlNode *Tilemap::getFirstChildForName(xmlNode *parent, const char *name) {
    
	xmlNode *child = parent->children->next;
    
	while  (child != NULL) {
		if (!strcmp((const char*)child->name, name))
			return child;
        
		child = child->next;
	}
    
	return NULL;
}

std::vector<xmlNode *> Tilemap::getChildrenForName(xmlNode *parent, const char *name) {
    
    std::vector<xmlNode *> ret;
    
	xmlNode *child = parent->children->next;
    
	while (child != NULL) {
		if (!strcmp((const char*)child->name, name))
			ret.push_back(child);
        
		child = child->next;
	}
    
	return ret;
}

char *Tilemap::getXmlAttribute(xmlNode *node, const char *name) {
    
	xmlAttr *attrs = node->properties;
    
	while (attrs != NULL) {
		if (!strcmp((const char*)attrs->name, name))
			return (char *)attrs->children->content;
        
		attrs = attrs->next;
	}
    
	return NULL;
}

void Tilemap::decodeLayerData(xmlNode *dataNode, int layerIdx) {
    
	char *encoding = getXmlAttribute(dataNode, "encoding");
	if (!encoding) {
        
		int i = 0;
        int j = 0;
        
        std::vector<xmlNode *> tiles = getChildrenForName(dataNode, "tile");
        std::vector<xmlNode *>::iterator it;
        
        for(it = tiles.begin(); it < tiles.end(); ++it) {
            
            xmlNode *tileNode = (xmlNode *) *it;
            char *gid = getXmlAttribute(tileNode, "gid");
            setTileAt(atoi(gid), i, j, layerIdx);
            i++;
            
            if (i >= mapWidth) {
                i = 0;
                j++;
            }
        }
        
	} else {
        
        fprintf(stderr, "Error: unknown compression format");
		return;
    }
}
