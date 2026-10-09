/**
 * Author: chilli, Takanori MAEHARA; adapted to the team interface
 * License: CC0
 * Summary: Intersection area of a disk and a polygon
 * Description: c is the disk; p is a simple polygon in boundary order.
 *  Either orientation works; returns nonnegative area. Repeated adjacent
 *  vertices and a repeated closing vertex are allowed. Does not change p.
 *  Finite long double coordinates/radius; rounding near tangencies.
 * Time: O(n)
 * Memory: O(1) extra
 * Usage: ld area = circlePolygonArea(c,p);
 */
#pragma once
#include "Circle2.h"

ld circlePolygonArea(Circle c, const vec<P2>& p) {
    assert(c.r >= 0);
    if (c.r == 0 || sz(p) < 3) return 0;
    auto angle = [](P2 a,P2 b) {
        return atan2l(a.cross(b),a.dot(b));
    };
    auto sector = [&](P2 a,P2 b) {
        return angle(a,b)*c.r*c.r/2;
    };
    auto edge = [&](P2 a,P2 b) {
        P2 d = b-a;
        if (d.dist2() == 0) return ld(0);
        ld u = d.dot(a)/d.dist2();
        ld v = (a.dist2()-c.r*c.r)/d.dist2();
        ld det = u*u-v;
        if (det <= 0) return sector(a,b);
        ld l = max(ld(0),-u-sqrtl(det));
        ld r = min(ld(1),-u+sqrtl(det));
        if (l >= r) return sector(a,b);
        P2 x = a+d*l, y = a+d*r;
        return sector(a,x)+x.cross(y)/2+sector(y,b);
    };
    ld sum = 0;
    rep(i,0,sz(p)) sum += edge(p[i]-c.c,p[(i+1)%sz(p)]-c.c);
    return abs(sum);
}
