/**
 * Author: Team reference
 * Summary: BFS distances, paths and bipartite colouring
 * Description: Unit-cost graph g[u]=outgoing neighbours; bfs(g,src) accepts one or several sources and returns (dist,parent). dist=-1 means unreachable; parent=-1 at sources and unreachable vertices. Duplicate/empty sources are allowed. restorePath(s,t,parent) returns s..t, or empty if the chain does not reach s; parents must be acyclic. bipartite(g) expects an undirected graph and returns (ok,color), checking every component and self-loop; colors 0/1 are valid only if ok. BFS/colouring $O(V+E+|src|)$, path $O(V)$; extra space $O(V)$.
 * Usage: vec<vec<int>> g{{1},{0,2},{1}};
 *    auto [d,p] = bfs(g,{0}); // d={0,1,2}
 *    auto path = restorePath(0,2,p);
 *    auto [ok,color] = bipartite(g);
 
 */
#pragma once

pair<vec<int>,vec<int>> bfs(const vec<vec<int>>& g,
		const vec<int>& src) {
	vec<int> dist(sz(g),-1), parent(sz(g),-1), q;
	for (int s : src) {
		assert(0 <= s && s < sz(g));
		if (dist[s] < 0) dist[s] = 0, q.pb(s);
	}
	rep(i,0,sz(q)) {
		int v = q[i];
		for (int u : g[v]) if (dist[u] < 0) {
			dist[u] = dist[v]+1; parent[u] = v;
			q.pb(u);
		}
	}
	return {dist,parent};
}
vec<int> restorePath(int s, int t, const vec<int>& parent) {
	assert(0 <= s && s < sz(parent));
	assert(0 <= t && t < sz(parent));
	vec<int> path;
	for (int v = t; v >= 0; v = parent[v]) {
		path.pb(v);
		if (v == s) {
			reverse(path.begin(),path.end());
			return path;
		}
	}
	return {};
}
pair<bool,vec<int>> bipartite(const vec<vec<int>>& g) {
	vec<int> color(sz(g),-1), q;
	rep(s,0,sz(g)) {
		if (color[s] >= 0) continue;
		color[s] = 0; q = {s};
		rep(i,0,sz(q)) {
			int v = q[i];
			for (int u : g[v]) {
				if (color[u] < 0) {
					color[u] = color[v]^1; q.pb(u);
				} else if (color[u] == color[v])
					return {false,color};
			}
		}
	}
	return {true,color};
}
