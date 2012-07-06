//
//  File.cpp
//  LPC2012Game
//
//  Created by Fábio Rodella on 7/6/12.
//  Copyright (c) 2012 Crocodella. All rights reserved.
//

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
