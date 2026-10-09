/**
 * Author: jet notebook
 * Summary: Euler trail: use each edge exactly once
 * Description: n vertices, edges e[i]=(u,v), supplied once even if undirected. directed selects orientation. Returns (vertices,edgeIDs) in trail order: E+1 vertices, E IDs; empty pair means impossible from src. With no edges returns one vertex src and no edge IDs. Self-loops/parallel edges allowed, vertices may repeat. Undirected: 0 or 2 odd degrees, start at an odd vertex if 2. Directed: out-in is +1 at start and -1 at end, or all balanced. A circuit starts at a vertex incident to an edge. Checks degrees and that every edge is used; time/space $O(V+E)$.
 * Usage: vec<pair<int,int>> e{{0,1},{1,2}};
 *    auto [v,ids]=eulerTrail(3,e,0,false);
 *    // v={0,1,2}, ids={0,1}
 
 */
#pragma once

pair<vec<int>,vec<int>> eulerTrail(int n,
		const vec<pair<int,int>>& e,
		int src, bool directed) {
	if (src < 0 || src >= n) return {};
	vec<vec<pair<int,int>>> g(n);
	vec<int> deg(n);
	rep(i,0,sz(e)) {
		auto [u,v] = e[i];
		g[u].pb({v,i}); ++deg[u];
		if (directed) --deg[v];
		else g[v].pb({u,i}), ++deg[v];
	}
	int pos = 0, neg = 0;
	rep(v,0,n) {
		if (directed) {
			if (deg[v] == 1 && v == src) ++pos;
			else if (deg[v] == -1) ++neg;
			else if (deg[v]) return {};
		} else pos += deg[v]&1;
	}
	if (directed && pos != neg) return {};
	if (!directed && pos &&
			(pos != 2 || !(deg[src]&1))) return {};
	vec<int> it(n), used(sz(e)), path, ids;
	vec<pair<int,int>> st{{src,-1}};
	while (!st.empty()) {
		auto [v,id] = st.back();
		while (it[v] < sz(g[v]) &&
				used[g[v][it[v]].second]) ++it[v];
		if (it[v] == sz(g[v])) {
			path.pb(v);
			if (id >= 0) ids.pb(id);
			st.pop_back();
		} else {
			auto [u,k] = g[v][it[v]++];
			used[k] = 1; st.pb({u,k});
		}
	}
	if (sz(ids) != sz(e)) return {};
	reverse(path.begin(),path.end());
	reverse(ids.begin(),ids.end());
	return {move(path),move(ids)};
}
