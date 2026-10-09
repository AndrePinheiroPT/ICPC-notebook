/**
 * Author: jet notebook
 * Summary: Maximum flow and minimum s-t cut
 * Description: add(u,v,capacity) creates a directed edge and its zero-capacity reverse. flow(s,t) sends maximum additional flow and changes residual capacities in g; repeated calls continue the same network. Returns units, not distance. Save the (u,index) handle returned by add: original capacity minus g[u][index].cap is its net flow. After a full flow call, minCut(s) returns a 0/1 vec of vertices reachable from s through positive residual capacity; original edges crossing 1 to 0 form a minimum cut. rev indexes the reverse edge; lev is BFS depth, it is DFS cursor. Require distinct valid s,t, nonnegative capacities, total flow fitting ll. Time $O(V^2E)$, space $O(V+E)$; recursive DFS depth $O(V)$.
 * Usage: DinicFlow f(3);
 *    auto edge=f.add(0,1,5); f.add(1,2,3);
 *    ll sent=f.flow(0,2); // 3
 *    auto side=f.minCut(0); // {1,1,0}
 
 */
#pragma once

struct DinicFlow {
	struct Edge { int to, rev; ll cap; };
	vec<vec<Edge>> g;
	vec<int> lev, it;
	DinicFlow(int n) : g(n), lev(n), it(n) {}
	pair<int,int> add(int u, int v, ll cap) {
		assert(cap >= 0);
		int a = sz(g[u]), b = sz(g[v]);
		g[u].pb({v,b+(u==v),cap});
		g[v].pb({u,a,0});
		return {u,a};
	}
	vec<int> minCut(int s) const {
		vec<int> side(sz(g)), q{s}; side[s]=1;
		rep(i,0,sz(q)) for (auto e : g[q[i]])
			if (e.cap>0 && !side[e.to])
				side[e.to]=1, q.pb(e.to);
		return side;
	}
	bool bfs(int s, int t) {
		fill(lev.begin(), lev.end(), -1);
		queue<int> q;
		q.push(s); lev[s] = 0;
		while (!q.empty()) {
			int u = q.front(); q.pop();
			for (auto e : g[u])
				if (e.cap > 0 && lev[e.to] < 0) {
					lev[e.to] = lev[u]+1;
					q.push(e.to);
				}
		}
		return lev[t] >= 0;
	}
	ll dfs(int u, int t, ll f) {
		if (u == t) return f;
		for (int& i = it[u]; i < sz(g[u]); ++i) {
			auto& e = g[u][i];
			if (e.cap <= 0 || lev[e.to] != lev[u]+1)
				continue;
			ll sent = dfs(e.to, t, min(f,e.cap));
			if (sent > 0) {
				e.cap -= sent; g[e.to][e.rev].cap += sent;
				return sent;
			}
		}
		return 0;
	}
	ll flow(int s, int t) {
		assert(0 <= s && s < sz(g));
		assert(0 <= t && t < sz(g) && s != t);
		ll ans = 0;
		while (bfs(s,t)) {
			fill(it.begin(), it.end(), 0);
			while (ll sent = dfs(s,t,
					LLONG_MAX)) ans += sent;
		}
		return ans;
	}
};
