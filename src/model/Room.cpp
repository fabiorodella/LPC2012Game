
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

#include <algorithm>

#include "Room.h"

Room::~Room() {

    std::vector<POI *>::iterator itPOI;
    for (itPOI = pointsOfInterest.begin(); itPOI < pointsOfInterest.end(); ++itPOI) {
        delete *itPOI;
    }
    
    pointsOfInterest.clear();
}

std::vector<POI *> Room::getPointsOfInterest(Interest interest, bool notSearchedOnly) {
    
    std::vector<POI *> ret;
    
    std::vector<POI *>::iterator it;
    for (it = pointsOfInterest.begin(); it < pointsOfInterest.end(); ++it) {
        POI *poi = (POI *) *it;
        if (poi->interest == interest) {
            if (notSearchedOnly && poi->searchedByMurderer) {
                continue;
            }
            ret.push_back(poi);
        }
    }
    
    return ret;
}

void Room::removePointOfInterest(POI *poi) {
    
    pointsOfInterest.erase(std::remove(pointsOfInterest.begin(), pointsOfInterest.end(), poi), pointsOfInterest.end());
}