/**
 * Author: jet notebook
 * Summary: Send a required or maximum flow at minimum cost
 * Description: add(u,v,cap,cost): directed capacity and cost per unit. flow(s,t,limit) maximises additional units up to limit, then minimises their total cost; returns (sent,totalCost). For exactly k units, pass k and check sent==k. Changes residual capacities; repeated calls continue the network and return incremental results. Reverse edges cancel earlier choices. Potentials h allow Dijkstra despite negative costs; pv/pe remember the previous vertex/edge. Negative edges allowed, but no negative residual cycle anywhere. Require s!=t, integral cap>=0, limit>=0, all intermediate sums/products fitting ll and distance/potential magnitudes below INF. Time $O(V+VE+A(E+V)\log(V+E+1))$ for A augmentations; space $O(V+E)$.
 * Usage: MinCostFlow f(3);
 *    f.add(0,1,2,3); f.add(1,2,2,1);
 *    auto [sent,cost]=f.flow(0,2,2); // {2,8}
 
 */
#pragma once

struct MinCostFlow {
	struct Edge { int to, rev; ll cap, cost; };
	const ll INF = LLONG_MAX/4;
	vec<vec<Edge>> g;
	MinCostFlow(int n) : g(n) {}
	void add(int u, int v, ll cap, ll cost) {
		assert(cap >= 0 && -INF < cost && cost < INF);
		int a = sz(g[u]), b = sz(g[v]);
		g[u].pb({v,b+(u==v),cap,cost});
		g[v].pb({u,a,0,-cost});
	}
	pair<ll,ll> flow(int s, int t,
		ll lim = LLONG_MAX) {
		int n = sz(g);
		assert(0 <= s && s < n && 0 <= t && t < n);
		assert(s != t && lim >= 0);
		vec<ll> h(n), d(n);
		vec<int> pv(n,-1), pe(n,-1);
		rep(k,0,n) {
			bool change = false;
			rep(u,0,n)
				for (auto e : g[u]) if (e.cap > 0)
					if (h[u]+e.cost < h[e.to]) {
						h[e.to] = h[u]+e.cost;
						change = true;
					}
			if (!change) break;
			assert(k+1 < n); // Sem ciclo negativo.
		}
		ll sent = 0, totalCost = 0;
		while (sent < lim) {
			fill(d.begin(), d.end(), INF);
			priority_queue<pair<ll,int>,
				vec<pair<ll,int>>,
				greater<pair<ll,int>>> q;
			d[s] = 0; q.push({0,s});
			while (!q.empty()) {
				auto [du,u] = q.top(); q.pop();
				if (du != d[u]) continue;
				rep(i,0,sz(g[u])) {
					auto e = g[u][i];
					if (e.cap <= 0) continue;
					ll reduced = du+e.cost+h[u]-h[e.to];
					if (reduced < d[e.to]) {
						d[e.to] = reduced;
						pv[e.to] = u; pe[e.to] = i;
						q.push({reduced,e.to});
					}
				}
			}
			if (d[t] == INF) break;
			rep(v,0,n)
				if (d[v] < INF) h[v] += d[v];
			ll amount = lim-sent;
			for (int v = t; v != s; v = pv[v])
				amount = min(amount,g[pv[v]][pe[v]].cap);
			for (int v = t; v != s; v = pv[v]) {
				auto& e = g[pv[v]][pe[v]];
				e.cap -= amount;
				g[v][e.rev].cap += amount;
			}
			sent += amount; totalCost += amount*(h[t]-h[s]);
		}
		return {sent,totalCost};
	}
};
