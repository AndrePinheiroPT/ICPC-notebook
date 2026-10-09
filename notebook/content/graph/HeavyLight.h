/**
 * Author: jet notebook
 * Summary: Tree path queries via array intervals
 * Description: Undirected connected tree g; root chosen as in TreeTour. Inherits par/dep/tin/tout/ord, reordered so heavy paths are consecutive; subtree(v) remains [tin[v],tout[v]). The example uses SegTree (SegmentTree.h), storing vertex values at tin[v]. path(u,v,f) calls f(l,r) for disjoint pieces covering the path; combine queries or apply updates in f. With edges=true, exclude the LCA and store each edge at its child's position. Segment order is unspecified: use only commutative aggregates unless adding direction handling. Build/space $O(V)$; $O(\log V)$ callbacks, hence typically $O(\log^2 V)$ per query/update.
 * Usage: HeavyLight h(g,0);
 *    ll ans=SegTree::unit;
 *    h.path(u,v,[&](int l,int r){ans=max(ans,seg.query(l,r));});
 
 */
#pragma once
#include "TreeTour.h"

struct HeavyLight : TreeTour {
	vec<int> head;
	HeavyLight(const vec<vec<int>>& g, int root = 0)
		: TreeTour(g,root), head(sz(g)) {
		if (g.empty()) return;
		vec<int> heavy(sz(g),-1);
		rep(v,0,sz(g))
			for (int u : g[v]) if (par[u] == v) {
				int w = heavy[v];
				if (w < 0 ||
						tout[u]-tin[u] > tout[w]-tin[w])
					heavy[v] = u;
			}
		vec<int> st{root}; ord.clear();
		while (!st.empty()) {
			int v = st.back(); st.pop_back();
			for (int u = v; u >= 0; u = heavy[u]) {
				head[u] = v;
				tin[u] = sz(ord); ord.pb(u);
				tout[u] = tin[u]+1;
				for (int w : g[u])
					if (par[w] == u && w != heavy[u])
						st.pb(w);
			}
		}
		for (int i = sz(ord); i--;) {
			int v = ord[i];
			if (par[v] >= 0)
				tout[par[v]] = max(tout[par[v]],tout[v]);
		}
	}
	template<class F>
	void path(int u, int v, F f,
			bool edges = false) const {
		while (head[u] != head[v]) {
			if (dep[head[u]] < dep[head[v]]) swap(u,v);
			f(tin[head[u]],tin[u]+1);
			u = par[head[u]];
		}
		if (dep[u] > dep[v]) swap(u,v);
		int l = tin[u]+int(edges), r = tin[v]+1;
		if (l < r) f(l,r);
	}
};
