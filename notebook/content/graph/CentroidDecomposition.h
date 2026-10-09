/**
 * Author: Team reference, based on Andre notebook's centroid decomposition
 * Summary: Balanced tree decomposition for distance-to-active queries
 * Description: Input is an undirected connected unweighted tree. parent[c] is c's parent in the centroid tree, -1 at its root. path[v] lists (centroid,distance) for all centroid ancestors of original vertex v, including v itself. Each list has O(log V) entries. Example: initialise best[c]=INF; activating v applies best[c]=min(best[c],d) for (c,d) in path[v]. Nearest active distance from u is min(best[c]+d) over path[u]. This recipe supports activations only; deletions need multisets or another container. Iterative component traversals avoid linear recursion depth. Build/space $O(V\log V)$; each activation/query $O(\log V)$.
 * Usage: CentroidDecomposition cd(g);
 *   vec<int> best(sz(g),INT_MAX/2);
 *   for (auto [c,d]:cd.path[v]) best[c]=min(best[c],d);
 *   int ans=INT_MAX/2;
 *   for (auto [c,d]:cd.path[u]) ans=min(ans,best[c]+d);
 
 */
#pragma once
struct CentroidDecomposition {
	vec<int> parent;
	vec<vec<pair<int,int>>> path;
	CentroidDecomposition(const vec<vec<int>>& g)
		: parent(sz(g),-1), path(sz(g)) {
		int n=sz(g);
		if (!n) return;
		vec<int> used(n), par(n,-1), sub(n);
		vec<pair<int,int>> tasks{{0,-1}};
		while (!tasks.empty()) {
			auto [s,p]=tasks.back(); tasks.pop_back();
			vec<int> ord{s}; par[s]=-1;
			rep(i,0,sz(ord)) {
				int v=ord[i]; sub[v]=1;
				for (int u:g[v]) if (!used[u] && u!=par[v])
					par[u]=v, ord.pb(u);
			}
			for (int i=sz(ord);i--;) {
				int v=ord[i];
				if (par[v]>=0) sub[par[v]]+=sub[v];
			}
			int c=-1;
			for (int v:ord) {
				int largest=sz(ord)-sub[v];
				for (int u:g[v]) if (!used[u] && par[u]==v)
					largest=max(largest,sub[u]);
				if (largest<=sz(ord)/2) {c=v; break;}
			}
			parent[c]=p;
			vec<tuple<int,int,int>> walk{{c,-1,0}};
			while (!walk.empty()) {
				auto [v,pv,d]=walk.back(); walk.pop_back();
				path[v].pb({c,d});
				for (int u:g[v]) if (!used[u] && u!=pv)
					walk.pb({u,v,d+1});
			}
			used[c]=1;
			for (int u:g[c]) if (!used[u]) tasks.pb({u,c});
		}
	}
};
