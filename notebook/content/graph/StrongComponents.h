/**
 * Author: jet notebook
 * Summary: Strongly connected components and their DAG
 * Description: Directed adjacency g[u]. kosaraju(g) returns comp[v], grouping mutually reachable vertices. Component IDs are contiguous and increase along every edge between different components. condensation(g,comp) returns the DAG of components without duplicates. Both DFS passes are iterative, safe on long chains. Neither changes g. Each function uses $O(V+E)$ time/space; TwoSAT relies on this component ordering.
 * Usage: vec<vec<int>> g{{1},{0,2},{3},{2}};
 *    auto comp=kosaraju(g); // {0,0,1,1}
 *    auto dag=condensation(g,comp); // {{1},{}}
 
 */
#pragma once

vec<int> kosaraju(const vec<vec<int>>& g) {
	int n = sz(g), count = 0;
	vec<vec<int>> rev(n);
	rep(v,0,n)
		for (int u : g[v]) rev[u].pb(v);
	vec<int> seen(n), next(n), order, comp(n,-1), st;
	rep(s,0,n) if (!seen[s]) {
		seen[s] = 1; st.pb(s);
		while (!st.empty()) {
			int v = st.back();
			if (next[v] == sz(g[v])) {
				order.pb(v); st.pop_back();
			} else {
				int u = g[v][next[v]++];
				if (!seen[u]) seen[u] = 1, st.pb(u);
			}
		}
	}
	for (int i = n; i--;) {
		int s = order[i];
		if (comp[s] >= 0) continue;
		comp[s] = count; st.pb(s);
		while (!st.empty()) {
			int v = st.back(); st.pop_back();
			for (int u : rev[v]) if (comp[u] < 0) {
				comp[u] = count; st.pb(u);
			}
		}
		++count;
	}
	return comp;
}
vec<vec<int>> condensation(const vec<vec<int>>& g,
		const vec<int>& comp) {
	int count = 0;
	for (int c : comp) count = max(count,c+1);
	vec<vec<int>> groups(count), dag(count);
	rep(v,0,sz(g))
		groups[comp[v]].pb(v);
	vec<int> mark(count,-1);
	rep(c,0,count)
		for (int v : groups[c]) for (int u : g[v]) {
			int d = comp[u];
			if (d != c && mark[d] != c) {
				mark[d] = c; dag[c].pb(d);
			}
		}
	return dag;
}
