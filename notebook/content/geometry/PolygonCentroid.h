/**
 * Author: Ulf Lundstrom; adapted to the team interface
 * License: CC0
 * Summary: Centre of mass of a uniform polygon
 * Description: p contains simple-polygon vertices in boundary order,
 *  clockwise or counterclockwise. Returns the area centroid;
 *  nullopt for fewer than 3 vertices or zero signed area.
 *  Finite real arithmetic; nearly zero area is ill-conditioned.
 * Time: O(n)
 * Memory: O(1) extra
 * Usage: auto centre = polygonCentroid(p);
 */
#pragma once
#include "Predicates.h"

optional<P2> polygonCentroid(const vec<P2>& p) {
    if (sz(p) < 3) return nullopt;
    P2 sum(0,0);
    ld area2 = 0;
    rep(i,0,sz(p)) {
        P2 a = p[i], b = p[(i+1)%sz(p)];
        ld cross = a.cross(b);
        area2 += cross;
        sum = sum+(a+b)*cross;
    }
    if (area2 == 0) return nullopt;
    return sum/(3*area2);
}
