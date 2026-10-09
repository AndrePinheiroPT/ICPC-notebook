/**
 * Author: jet notebook
 * Summary: Bridges and articulation points
 * Description: Undirected edges e[i]=(u,v), each supplied once; n vertices. bridges contains edge IDs whose removal increases component count; cut[v]=1 marks vertices with the same property. Handles disconnected graphs, self-loops and parallel edges. tin is DFS discovery time; low is the earliest discovery reachable below a vertex using a back edge. Parent edges are tracked by ID. Iterative DFS; time/space $O(V+E)$.
 * Usage: vec<pair<int,int>> e{{0,1},{1,2},{2,0},{1,3}};
 *    Lowlink b(4,e); // bridges={3}, cut[1]=1
 
 */
#pragma once

struct Lowlink {
	vec<int> bridges, cut;
	Lowlink(int n, const vec<pair<int,int>>& e)
		: cut(n) {
		vec<vec<pair<int,int>>> g(n);
		rep(i,0,sz(e)) {
			auto [u,v] = e[i];
			g[u].pb({v,i});
			g[v].pb({u,i});
		}
		vec<int> tin(n,-1), low(n), par(n,-1), pe(n,-1);
		vec<int> it(n), child(n), st;
		int timer = 0;
		rep(s,0,n) if (tin[s] < 0) {
			tin[s] = low[s] = timer++;
			st.pb(s);
			while (!st.empty()) {
				int v = st.back();
				if (it[v] < sz(g[v])) {
					auto [u,id] = g[v][it[v]++];
					if (id == pe[v]) continue;
					if (tin[u] < 0) {
						par[u] = v; pe[u] = id; ++child[v];
						tin[u] = low[u] = timer++;
						st.pb(u);
					} else low[v] = min(low[v],tin[u]);
				} else {
					st.pop_back();
					int p = par[v];
					if (p < 0) cut[v] = child[v] > 1;
					else {
						low[p] = min(low[p],low[v]);
						if (low[v] > tin[p])
							bridges.pb(pe[v]);
						if (par[p] >= 0 && low[v] >= tin[p])
							cut[p] = 1;
					}
				}
			}
		}
	}
};
