/**
 * Author: jet notebook
 * Summary: Flatten rooted subtrees into array intervals
 * Description: g is an undirected connected tree; root selects its root. Empty input allowed. par[root]=-1, dep[v] counts edges from root. ord[i] is the vertex at position i; v's subtree is exactly ord[tin[v]..tout[v]), including v. Store a vertex value at position tin[v] for Fenwick/segment-tree queries. Reverse ord processes children before parents. Iterative build, time/space $O(V)$.
 * Usage: vec<vec<int>> g{{1},{0,2,3},{1},{1}};
 *    TreeTour t(g);
 *    // subtree(1): [t.tin[1],t.tout[1]) = [1,4)
 
 */
#pragma once

struct TreeTour {
	vec<int> par, dep, tin, tout, ord;
	TreeTour(const vec<vec<int>>& g, int root = 0)
		: par(sz(g),-1), dep(sz(g)),
		  tin(sz(g)), tout(sz(g)) {
		if (g.empty()) return;
		vec<int> st{root}, size(sz(g),1);
		while (!st.empty()) {
			int v = st.back(); st.pop_back();
			tin[v] = sz(ord); ord.pb(v);
			for (int u : g[v]) if (u != par[v]) {
				par[u] = v; dep[u] = dep[v]+1;
				st.pb(u);
			}
		}
		for (int i = sz(ord); i--;) {
			int v = ord[i];
			tout[v] = tin[v]+size[v];
			if (par[v] >= 0)
				size[par[v]] += size[v];
		}
	}
};
