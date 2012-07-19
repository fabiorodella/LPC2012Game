
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

#ifndef LPC2012Game_Utils_h
#define LPC2012Game_Utils_h

static std::string timeToString(long time, bool includeSeconds) {
    
    int hr = time / (60 * 60);
	int min = (((int)time / 60) % 60);
	int sec = (int)time % 60; 
    
    char buff[10];
    
    if (includeSeconds) {
        sprintf(buff, "%.2d:%.2d:%.2d", hr, min, sec);
    } else {
        sprintf(buff, "%.2d:%.2d", hr, min);
    }
    
    std::string ret = buff;
    return ret;
}

#endif
