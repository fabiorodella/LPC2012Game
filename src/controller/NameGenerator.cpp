
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

#include "NameGenerator.h"
#include "XmlHelper.h"

std::string generateName(bool male) {
    
    std::string ret;
    
    const char *file;
    
    if (male) {
        file = "res/maleNames.xml";
    } else {
        file = "res/femaleNames.xml";
    }
    
    xmlDoc *doc = xmlReadFile(file, NULL, 0);
    
    xmlNode *root = xmlDocGetRootElement(doc);
    
    xmlNode *namesNode = xmlGetFirstChildForName(root, "first");
    
    std::vector<xmlNode *> nameNodes = xmlGetChildrenForName(namesNode, "name");
    int idx = rand() % nameNodes.size();
    
    xmlNode *nameNode = nameNodes[idx];
    ret = std::string((char *) nameNode->children->content);
    
    ret.append(" ");
    
    namesNode = xmlGetFirstChildForName(root, "last");
    
    nameNodes = xmlGetChildrenForName(namesNode, "name");
    idx = rand() % nameNodes.size();
    
    nameNode = nameNodes[idx];
    ret.append((char *) nameNode->children->content);
    
    return ret;
}
