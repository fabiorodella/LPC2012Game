
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

#include "XmlHelper.h"

xmlNode *xmlGetFirstChildForName(xmlNode *parent, const char *name) {
    
	xmlNode *child = parent->children->next;
    
	while  (child != NULL) {
		if (!strcmp((const char*)child->name, name))
			return child;
        
		child = child->next;
	}
    
	return NULL;
}

std::vector<xmlNode *> xmlGetChildrenForName(xmlNode *parent, const char *name) {
            
    std::vector<xmlNode *> ret;
    
    if (parent->children != NULL) {
        
        xmlNode *child = parent->children->next;
        
        while (child != NULL) {
            if (!strcmp((const char*)child->name, name))
                ret.push_back(child);
            
            child = child->next;
        }
    }
    
	return ret;
}

char *xmlGetAttribute(xmlNode *node, const char *name) {
    
	xmlAttr *attrs = node->properties;
    
	while (attrs != NULL) {
		if (!strcmp((const char*)attrs->name, name))
			return (char *)attrs->children->content;
        
		attrs = attrs->next;
	}
    
	return NULL;
}