/**
 * Author: Simon Lindholm
 * License: CC0
 * Summary: Shortest paths and reachable negative cycles
 * Description: Create fresh nodes(V) and edges (from,to,intWeight); call BF::bellmanFord(nodes,edges,s). Reads nodes[v].dist and .prev (previous vertex). BF::inf means unreachable; -inf means a reachable negative cycle can reduce the cost to v without bound. Sorts edges in place; prev is meaningful only for finite distances and may not retain the usual source-parent convention when negative cycles exist. All intermediate finite sums must fit $(-inf,inf)$; $V(E+1)\max|w|<inf$ suffices, inf=LLONG\_MAX. Time $O(V(E+1)+E\log(E+1))$, extra space $O(\log(E+1))$.
 * Usage: vec<BF::Node> nodes(3);
 *    vec<BF::Ed> edges{{0,1,4},{1,2,-2}};
 *    BF::bellmanFord(nodes,edges,0);
 *    ll answer=nodes[2].dist; // 2
 
 */
#pragma once

namespace BF {
const ll inf = LLONG_MAX;
struct Ed {
	int a, b, w, s() {
		return a < b ? a : -a;
	}
};
struct Node { ll dist = inf; int prev = -1; };

void bellmanFord(vec<Node>& nodes,
		vec<Ed>& eds, int s) {
	nodes[s].dist = 0;
	sort(eds.begin(), eds.end(), [](Ed a, Ed b) {
		return a.s() < b.s();
	});

	int lim = sz(nodes) / 2 + 2;
	// /3+100 com vértices baralhados
	rep(i,0,lim)
		for (Ed ed : eds) {
			Node cur = nodes[ed.a],
				&dest = nodes[ed.b];
			if (abs(cur.dist) == inf) continue;
			ll d = cur.dist + ed.w;
			if (d < dest.dist) {
				dest.prev = ed.a;
				dest.dist = (i < lim-1 ? d : -inf);
			}
		}
	rep(i,0,lim)
		for (Ed e : eds) {
			if (nodes[e.a].dist == -inf)
				nodes[e.b].dist = -inf;
		}
}
} // namespace BF
