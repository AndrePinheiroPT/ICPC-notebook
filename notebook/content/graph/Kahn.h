/**
 * Author: jet notebook, based on KACTL
 * Summary: Topological order: prerequisites before dependants
 * Description: For u before v, put v in g[u]. Returns vertices in topological order if the directed graph is acyclic; fewer than V vertices means a cycle prevents a complete order. in counts unfinished prerequisites. A FIFO queue gives any valid order; use a min-heap for lexicographically smallest order. Time $O(V+E)$, extra space $O(V)$.
 * Usage: vec<vec<int>> g{{2},{2},{}};
 *    auto order=kahn(g); // {0,1,2}
 *    bool dag=sz(order)==sz(g);
 
 */
#pragma once

vec<int> kahn(const vec<vec<int>>& g) {
	vec<int> in(sz(g)), q;
	for (auto& adj : g) for (int v : adj) ++in[v];
	rep(v,0,sz(g))
		if (!in[v]) q.pb(v);
	rep(i,0,sz(q))
		for (int v : g[q[i]]) if (!--in[v])
			q.pb(v);
	return q;
}
