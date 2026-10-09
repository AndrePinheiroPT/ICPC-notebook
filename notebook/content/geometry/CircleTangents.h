/**
 * Author: Victor Lecomte, chilli; adapted to the team interface
 * License: CC0
 * Summary: Common tangents of two circles
 * Description: Returns pairs of tangency points (on a, on b).
 *  External tangents by default; inner=true asks for internal tangents.
 *  Radii are nonnegative; a zero radius represents a point.
 *  Empty means none, or coincident centres (infinite case not enumerated).
 *  At tangency the two points coincide: the line is perpendicular to b.c-a.c.
 *  Uses long double; nearly tangent inputs are sensitive to rounding.
 * Time: O(1)
 * Usage: auto lines = circleTangents(a,b);
 */
#pragma once
#include "Circle2.h"

vec<pair<P2,P2>> circleTangents(
        Circle a, Circle b, bool inner = false) {
    assert(a.r >= 0 && b.r >= 0);
    ld r2 = inner ? -b.r : b.r;
    P2 d = b.c-a.c;
    ld dr = a.r-r2, d2 = d.dist2();
    ld h2 = d2-dr*dr;
    if (d2 == 0 || h2 < 0) return {};
    vec<pair<P2,P2>> out;
    for (int sign : {-1,1}) {
        P2 v = (d*dr+d.perp()*(sqrtl(h2)*sign))/d2;
        out.pb({a.c+v*a.r,b.c+v*r2});
    }
    if (h2 == 0) out.pop_back();
    return out;
}
