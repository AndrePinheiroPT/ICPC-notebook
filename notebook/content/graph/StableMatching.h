/**
 * Author: Team reference, based on jet notebook
 * Summary: Stable pairing under two-sided preferences
 * Description: a[u] lists acceptable receivers in decreasing preference; b[v] lists acceptable proposers. Sides may differ in size; lists may be incomplete, but must contain distinct valid IDs and no ties. A pair is acceptable only if both list each other. Returns each proposer's receiver, -1 if unmatched. Gale--Shapley is proposer-optimal among stable matchings: no mutually acceptable pair prefer each other to their current partner (being unmatched is worst). Stability is not maximum total score. rank[v][u] is receiver v's preference index; next[u] is its next untried proposal. Time $O(nm+|a|+|b|+n+m)$ and space $O(nm+n+m)$; inputs unchanged.
 * Usage: vec<vec<int>> a{{0,1},{0}}, b{{1,0},{0}};
 *   auto partner=stableMatching(a,b); // {1,0}
 
 */
#pragma once
vec<int> stableMatching(const vec<vec<int>>& a,
		const vec<vec<int>>& b) {
	int n=sz(a), m=sz(b);
	vec<vec<int>> rank(m,vec<int>(n,n));
	rep(v,0,m) rep(i,0,sz(b[v])) {
		int u=b[v][i];
		assert(0<=u && u<n && rank[v][u]==n);
		rank[v][u]=i;
	}
	vec<int> l(n,-1), r(m,-1), next(n), free(n);
	iota(all(free),0);
	while (!free.empty()) {
		int u=free.back(); free.pop_back();
		while (next[u]<sz(a[u])) {
			int v=a[u][next[u]++];
			assert(0<=v && v<m);
			int w=r[v];
			if (rank[v][u]==n) continue;
			if (w<0 || rank[v][u]<rank[v][w]) {
				l[u]=v; r[v]=u;
				if (w>=0) l[w]=-1, free.pb(w);
				break;
			}
		}
	}
	return l;
}
