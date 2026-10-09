/**
 * Author: jet notebook
 * Summary: Farthest pair in a nonnegative weighted tree
 * Description: g[u] contains (to,weight), both directions of each tree edge. All weights must be nonnegative; use weight 1 if unweighted. Returns (length,endpointA,endpointB); empty tree returns (0,-1,-1), endpoints may coincide. Two iterative traversals find a farthest vertex, then a farthest vertex from it. T is inferred from weights and must hold every path sum; prefer ll. Time/space $O(V)$.
 * Usage: vec<vec<pair<int,ll>>> g{
 *        {{1,4}},{{0,4},{2,2}},{{1,2}}};
 *    auto [length,a,b]=treeDiameter(g); // length=6
 
 */
#pragma once

template<class T>
tuple<T,int,int> treeDiameter(
		const vec<vec<pair<int,T>>>& g) {
	int n = sz(g);
	if (!n) return {T{},-1,-1};
	auto far = [&](int s) {
		vec<T> d(n);
		vec<int> par(n,-1), q{s};
		int best = s;
		rep(i,0,sz(q)) {
			int v = q[i];
			if (d[v] > d[best]) best = v;
			for (auto [u,w] : g[v]) if (u != par[v]) {
				par[u] = v; d[u] = d[v]+w;
				q.pb(u);
			}
		}
		return pair{best,d[best]};
	};
	int a = far(0).first;
	auto [b,d] = far(a);
	return {d,a,b};
}
