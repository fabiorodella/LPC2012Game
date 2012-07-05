
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

inline Point pointMake(float px, float py) {
    Point p;
    p.x = px;
    p.y = py;
    return p;
}

inline Point pointOffset(Point p, float dx, float dy) {
    return pointMake(p.x + dx, p.y + dy);
}

inline bool pointEquals(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

inline bool pointEqualsIntegral(Point p1, Point p2) {
    int p1x = p1.x;
    int p1y = p1.y;
    int p2x = p2.x;
    int p2y = p2.y;
    return p1x == p2x && p1y == p2y;
}

inline Size sizeMake(float w, float h) {
    Size s;
    s.width = w;
    s.height = h;
    return s;
}

inline Rect rectMake(float ox, float oy, float w, float h) {
    Rect r;
    r.origin = pointMake(ox, oy);
    r.size = sizeMake(w, h);
    return r;
}

inline Rect rectOffset(Rect r, float dx, float dy) {
    return rectMake(r.origin.x + dx, r.origin.y + dy, r.size.width, r.size.height);
}

inline Point rectMidPoint(Rect r) {
    return pointOffset(r.origin, r.size.width / 2, r.size.height / 2);
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
