/**
 * Author: jet notebook
 * Summary: Shortest paths with weights 0 or 1
 * Description: g[u] contains (to,weight), weight is 0 or 1. src contains one or several sources, initially at distance 0. Returns (dist,parent) from the nearest source: INT\_MAX means unreachable; parent=-1 at sources/unreachable vertices. Sources may repeat or be empty. Weight 0 goes to the deque front, 1 to the back; ordinary BFS does not work for mixed weights. Use restorePath from Traversal.h. Time $O(V+E+|src|)$, extra space $O(V+E)$.
 * Usage: vec<vec<pair<int,int>>> g(3);
 *    g[0]={{1,0},{2,1}}; g[1]={{2,0}};
 *    auto [d,p] = zeroOneBfs(g,{0}); // {0,0,0}
 
 */
#pragma once

pair<vec<int>,vec<int>> zeroOneBfs(
		const vec<vec<pair<int,int>>>& g,
		const vec<int>& src) {
	vec<int> d(sz(g),INT_MAX), parent(sz(g),-1);
	deque<pair<int,int>> q;
	for (int s : src) {
		assert(0 <= s && s < sz(g));
		if (d[s] != 0) {
			d[s] = 0; q.pb({0,s});
		}
	}
	while (!q.empty()) {
		auto [dv,v] = q.front(); q.pop_front();
		if (dv != d[v]) continue;
		for (auto [u,w] : g[v]) {
			assert(w == 0 || w == 1);
			if (dv+w >= d[u]) continue;
			d[u] = dv+w; parent[u] = v;
			if (w) q.pb({d[u],u});
			else q.push_front({d[u],u});
		}
	}
	return {d,parent};
}
