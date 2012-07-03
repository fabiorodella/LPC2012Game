
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

#ifndef LPC2012Game_Geometry_h
#define LPC2012Game_Geometry_h

struct Point {
    float x;
    float y;
};

struct Size {
    float width;
    float height;
};

struct Rect {
    Point origin;
    Size size;
};

#define POINT_MAKE(__X__, __Y__) (Point){.x = __X__, .y = __Y__}
#define SIZE_MAKE(__W__,__H__) (Size){.width = __W__, .height = __H__}
#define RECT_MAKE(__X__, __Y__, __W__, __H__) (Rect){.origin = (Point){.x = __X__, .y = __Y__}, .size = (Size){.width = __W__, .height = __H__}}

inline Point pointOffset(Point p, float dx, float dy) {
    return POINT_MAKE(p.x + dx, p.y + dy);
}

inline Rect rectOffset(Rect r, float dx, float dy) {
    return RECT_MAKE(r.origin.x + dx, r.origin.y + dy, r.size.width, r.size.height);
}

inline bool rectContainsPoint(Rect r, Point p) {
    return p.x >= r.origin.x && p.x <= (r.origin.x + r.size.width) &&
        p.y >= r.origin.y && p.y <= (r.origin.y + r.size.height);
}

inline bool rectIntersectsRect(Rect r1, Rect r2) {
    return r1.origin.x < (r2.origin.x + r2.size.width) &&
        (r1.origin.x + r1.size.width) > r2.origin.x &&
        r1.origin.y < (r2.origin.y + r2.size.height) &&
        (r1.origin.y + r1.size.height) > r2.origin.y;
}

#endif
