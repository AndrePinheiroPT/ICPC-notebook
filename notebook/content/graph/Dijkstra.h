/**
 * Author: jet notebook
 * Summary: Shortest paths with nonnegative edge weights
 * Description: g[u] contains (to,weight), weight>=0; add both directions for an undirected edge. dijkstra(g,s) or dijkstra(g,src) returns (dist,parent), from the nearest source; a source starts at cost 0. dist=inf means unreachable; finite answers must be <inf. parent=-1 at sources/unreachable vertices; use restorePath from Traversal.h. Parents change only on strict improvements. Heap version: $O((V+E)\log(V+E+1))$ time and $O(V+E)$ extra space. dijkstraDense(g,s) instead scans all vertices: $O(V^2+E)$ time, $O(V)$ extra space; useful for dense graphs.
 * Usage: vec<vec<pair<int,ll>>> g(3);
 *    g[0]={{1,7}}; g[1]={{2,2}};
 *    auto [d,p] = dijkstra(g,0); // {0,7,9}
 
 */
#pragma once

pair<vec<ll>,vec<int>> dijkstra(
		const vec<vec<pair<int,ll>>>& g,
		const vec<int>& src,
		ll inf = LLONG_MAX/4) {
	assert(inf > 0);
	vec<ll> dist(sz(g),inf);
	vec<int> parent(sz(g),-1);
	using P = pair<ll,int>;
	priority_queue<P,vec<P>,greater<P>> pq;
	for (int s : src) {
		assert(0 <= s && s < sz(g));
		if (dist[s] != 0) dist[s] = 0, pq.push({0,s});
	}
	while (!pq.empty()) {
		auto [du,u] = pq.top(); pq.pop();
		if (du != dist[u]) continue;
		for (auto [v,w] : g[u]) {
			assert(w >= 0);
			if (w >= inf-du) continue;
			ll nd = du+w;
			if (nd < dist[v]) {
				dist[v] = nd; parent[v] = u;
				pq.push({nd,v});
			}
		}
	}
	return {dist,parent};
}

pair<vec<ll>,vec<int>> dijkstra(
		const vec<vec<pair<int,ll>>>& g,
		int s, ll inf = LLONG_MAX/4) {
	return dijkstra(g,vec<int>{s},inf);
}
pair<vec<ll>,vec<int>> dijkstraDense(
		const vec<vec<pair<int,ll>>>& g,
		int s, ll inf = LLONG_MAX/4) {
	int n=sz(g);
	assert(0<=s && s<n && inf>0);
	vec<ll> d(n,inf);
	vec<int> parent(n,-1), used(n);
	d[s]=0;
	rep(i,0,n) {
		int u=-1;
		rep(v,0,n) if (!used[v] && (u<0 || d[v]<d[u])) u=v;
		if (u<0 || d[u]==inf) break;
		used[u]=1;
		for (auto [v,w] : g[u]) {
			assert(w>=0);
			if (w<inf-d[u] && d[u]+w<d[v]) {
				d[v]=d[u]+w; parent[v]=u;
			}
		}
	}
	return {d,parent};
}
