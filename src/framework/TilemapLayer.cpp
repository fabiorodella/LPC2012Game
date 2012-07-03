
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

#include "TilemapLayer.h"

std::vector<TilemapLayer *> TilemapLayer::parseTMXFile(const char *file) {
    
    // THIS IS NOT a complete TMX parser! It will only work with:
    // - orthogonal maps
    // - single tileset
    // - all layers with same size
    // - tile data without compression
    // - no flipped tiles
    // - firstGid = 1
    
    std::vector<TilemapLayer *> ret;
    
    xmlDoc *doc = xmlReadFile(file, NULL, 0);
    
    xmlNode *root = xmlDocGetRootElement(doc);
    
	int mapWidth = atoi(getXmlAttribute(root, "width"));
	int mapHeight = atoi(getXmlAttribute(root, "height"));
	int tileWidth = atoi(getXmlAttribute(root, "tilewidth"));
	int tileHeight = atoi(getXmlAttribute(root, "tileheight"));
    
    xmlNode *tilesetNode = getFirstChildForName(root, "tileset");
    xmlNode *imageNode = getFirstChildForName(tilesetNode, "image");
    
    const char *imageFile = getXmlAttribute(imageNode, "source");
    
    std::vector<xmlNode *> layerNodes = getChildrenForName(root, "layer");
    std::vector<xmlNode *>::iterator layerNodesIt;
    
    for(layerNodesIt = layerNodes.begin(); layerNodesIt < layerNodes.end(); ++layerNodesIt) {
        
        TilemapLayer *layer = new TilemapLayer();
        
        layer->mapSize = SIZE_MAKE(mapWidth, mapHeight);
        layer->tileSize = SIZE_MAKE(tileWidth, tileHeight);
        
        layer->tileset = new Spritesheet(imageFile, tileWidth, tileHeight);
        
        layer->data = new short[mapWidth * mapHeight];
        
        layer->collision = false;
        
        xmlNode *layerNode = (xmlNode *) *layerNodesIt;
        decodeLayerData(getFirstChildForName(layerNode, "data"), layer);
        
        xmlNode *propertiesNode = getFirstChildForName(layerNode, "properties");
        
        if (propertiesNode != NULL) {
            
            std::vector<xmlNode *> propertyNodes = getChildrenForName(propertiesNode, "property");
            std::vector<xmlNode *>::iterator propertyNodesIt;
            
            for(propertyNodesIt = propertyNodes.begin(); propertyNodesIt < propertyNodes.end(); ++propertyNodesIt) {
                
                xmlNode *propertyNode = (xmlNode *) *propertyNodesIt;
                const char *name = getXmlAttribute(propertyNode, "name");
                
                if (!strcmp(name, LAYER_Z_ORDER_PROPERTY)) {
                    
                    layer->zOrder = atoi(getXmlAttribute(propertyNode, "value"));
                    
                } else if (!strcmp(name, LAYER_COLLISION_PROPERTY)) {
                    
                    layer->collision = true;
                }
                
            }
        }
        
        ret.push_back(layer);
    }
    
    return ret;
}

TilemapLayer::~TilemapLayer() {
    
    delete data;
    delete tileset;
}

void TilemapLayer::draw() {
    
    //TODO for now, maps can only be positioned at origin (0,0) and
    // the anchor point is disregarded.
    
    float topX = 0;
    float topY = 0;
    
    int tilesX = 0;
    int tilesY = 0;
    
    if (camera != NULL) {
        
        topX = camera->getTop().x;
        topY = camera->getTop().y;
        
        tilesX = camera->getViewportSize().width / tileSize.width;
        tilesY = camera->getViewportSize().height / tileSize.height;
        
    } else {
        
        tilesX = al_get_display_width(al_get_current_display()) / tileSize.width;
        tilesY = al_get_display_height(al_get_current_display()) / tileSize.height;
    }
    
    int tx = topX / tileSize.width;
    int ty = topY / tileSize.height;
    
    int ox = -camera->getTop().x;
    int oy = -camera->getTop().y;
    
    al_hold_bitmap_drawing(true);
    
    for (int j = ty; j < (ty + tilesY + 2); j++) {
        for (int i = tx; i < (tx + tilesX + 2); i++) {
                                    
            if (i >= 0 && i < mapSize.width && j >= 0 && j < mapSize.height && getTileAt(i, j) > 0) {
            
                short tile = getTileAt(i, j) - 1;
                tileset->setFrame(tile);
                tileset->setPosition(POINT_MAKE(i * tileSize.width + ox, j * tileSize.height + oy));
                tileset->draw();
            }
        }
    }
    
    al_hold_bitmap_drawing(false);
}

void TilemapLayer::setTileAt(short tile, int x, int y) {
    
    int mw = mapSize.width;
    data[y * mw + x] = tile;
}

short TilemapLayer::getTileAt(int x, int y) {
    
    int mw = mapSize.width;
    return data[y * mw + x];
}

Size TilemapLayer::getSize() {
    
    return mapSize;
}

Size TilemapLayer::getBoundsSize() {
    
    return SIZE_MAKE(mapSize.width * tileSize.width, mapSize.height * tileSize.height);
}

Size TilemapLayer::getTileSize() {
    
    return tileSize;
}

bool TilemapLayer::isCollision() {
    
    return collision;
}

xmlNode *TilemapLayer::getFirstChildForName(xmlNode *parent, const char *name) {
    
	xmlNode *child = parent->children->next;
    
	while  (child != NULL) {
		if (!strcmp((const char*)child->name, name))
			return child;
        
		child = child->next;
	}
    
	return NULL;
}

std::vector<xmlNode *> TilemapLayer::getChildrenForName(xmlNode *parent, const char *name) {
    
    std::vector<xmlNode *> ret;
    
	xmlNode *child = parent->children->next;
    
	while (child != NULL) {
		if (!strcmp((const char*)child->name, name))
			ret.push_back(child);
        
		child = child->next;
	}
    
	return ret;
}

char *TilemapLayer::getXmlAttribute(xmlNode *node, const char *name) {
    
	xmlAttr *attrs = node->properties;
    
	while (attrs != NULL) {
		if (!strcmp((const char*)attrs->name, name))
			return (char *)attrs->children->content;
        
		attrs = attrs->next;
	}
    
	return NULL;
}

void TilemapLayer::decodeLayerData(xmlNode *dataNode, TilemapLayer *layer) {
    
	char *encoding = getXmlAttribute(dataNode, "encoding");
	if (!encoding) {
        
		int i = 0;
        int j = 0;
        
        std::vector<xmlNode *> tiles = getChildrenForName(dataNode, "tile");
        std::vector<xmlNode *>::iterator it;
        
        for(it = tiles.begin(); it < tiles.end(); ++it) {
            
            xmlNode *tileNode = (xmlNode *) *it;
            char *gid = getXmlAttribute(tileNode, "gid");
            layer->setTileAt(atoi(gid), i, j);
            i++;
            
            if (i >= layer->mapSize.width) {
                i = 0;
                j++;
            }
        }
        
	} else {
        
        fprintf(stderr, "Error: unknown compression format");
		return;
    }
}
