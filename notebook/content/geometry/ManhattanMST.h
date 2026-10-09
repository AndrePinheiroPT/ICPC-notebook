/**
 * Author: chilli, Takanori MAEHARA; adapted to the team interface
 * License: CC0
 * Summary: Sparse candidate edges for a Manhattan MST
 * Description: p contains integer points with $|coordinate|\le 10^{15}$.
 *  Returns at most 4n edges (weight,u,v), using ORIGINAL 0-based IDs.
 *  These edges contain an MST of the complete graph whose weights are
 *  abs(dx)+abs(dy). Run Kruskal on them; they are not the MST itself.
 *  Copies its input; duplicates and empty input are supported.
 * Time: O(n log n)
 * Memory: O(n)
 * Usage: auto candidates = manhattanEdges(points);
 */
#pragma once
#include "Point.h"

vec<tuple<ll,int,int>> manhattanEdges(vec<Point<ll>> p) {
    for (auto q : p) {
        assert(-1000000000000000LL <= q.x && q.x <= 1000000000000000LL);
        assert(-1000000000000000LL <= q.y && q.y <= 1000000000000000LL);
    }
    vec<int> id(sz(p));
    iota(all(id),0);
    vec<tuple<ll,int,int>> edges;
    rep(k,0,4) {
        sort(all(id),[&](int i,int j) {
            return p[i].x+p[i].y < p[j].x+p[j].y;
        });
        map<ll,int> sweep;
        for (int i : id) {
            auto it = sweep.lower_bound(-p[i].y);
            while (it != sweep.end()) {
                int j = it->second;
                auto d = p[i]-p[j];
                if (d.y > d.x) break;
                edges.pb({d.x+d.y,i,j});
                it = sweep.erase(it);
            }
            sweep[-p[i].y] = i;
        }
        for (auto& q : p)
            if (k&1) q.x = -q.x;
            else swap(q.x,q.y);
    }
    return edges;
}
