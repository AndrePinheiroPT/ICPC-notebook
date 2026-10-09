/**
 * Author: jet notebook
 * Summary: Maximum bipartite matching and minimum vertex cover
 * Description: n left and m right vertices, independently numbered from 0. add(u,v) marks a compatible left/right pair. solve() rebuilds a maximum-cardinality matching: l[u]=right partner, r[v]=left partner, -1 if unmatched; returns number of pairs. After solve, cover() returns (leftIDs,rightIDs) of a minimum vertex cover, touching every edge; their complements form a maximum independent set. Do not modify g between solve and cover. Isolated vertices/duplicate edges allowed; this does not optimise costs or preferences. Hopcroft--Karp: $O((V+E)\sqrt V)$ time, $O(V+E)$ space; cover linear. DFS depth $O(V)$.
 * Usage: BipartiteMatching b(2,3);
 *    b.add(0,1); b.add(1,1); b.add(1,2);
 *    int pairs=b.solve(); // 2
 *    auto [left,right]=b.cover();
 
 */
#pragma once

struct BipartiteMatching {
	int n, m;
	vec<vec<int>> g;
	vec<int> l, r, d;
	BipartiteMatching(int a, int b)
		: n(a), m(b), g(a), l(a,-1), r(b,-1),
		  d(a+1) {}
	void add(int u, int v) { g[u].pb(v); }
	bool bfs() {
		fill(d.begin(), d.end(), n+1);
		queue<int> q;
		rep(u,0,n) if (l[u] < 0)
			d[u] = 0, q.push(u);
		while (!q.empty()) {
			int u = q.front(); q.pop();
			if (d[u] >= d[n]) continue;
			for (int v : g[u]) {
				int w = r[v] < 0 ? n : r[v];
				if (d[w] == n+1) {
					d[w] = d[u]+1;
					if (w < n) q.push(w);
				}
			}
		}
		return d[n] <= n;
	}
	bool dfs(int u) {
		if (u == n) return true;
		for (int v : g[u]) {
			int w = r[v] < 0 ? n : r[v];
			if (d[w] == d[u]+1 && dfs(w)) {
				l[u] = v; r[v] = u; return true;
			}
		}
		d[u] = n+1;
		return false;
	}
	int solve() {
		fill(l.begin(), l.end(), -1);
		fill(r.begin(), r.end(), -1);
		int ans = 0;
		while (bfs()) rep(u,0,n)
			if (l[u] < 0 && dfs(u)) ++ans;
		return ans;
	}
	pair<vec<int>,vec<int>> cover() const {
		vec<int> a(n), b(m);
		queue<int> q;
		rep(u,0,n) if (l[u] < 0)
			a[u] = 1, q.push(u);
		while (!q.empty()) {
			int u = q.front(); q.pop();
			for (int v : g[u])
				if (l[u] != v && !b[v]) {
					b[v] = 1;
					int w = r[v];
					if (w >= 0 && !a[w])
						a[w] = 1, q.push(w);
				}
		}
		pair<vec<int>,vec<int>> ans;
		rep(u,0,n)
			if (!a[u]) ans.first.pb(u);
		rep(v,0,m)
			if (b[v]) ans.second.pb(v);
		return ans;
	}
};
