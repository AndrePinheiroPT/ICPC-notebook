/**
 * Author: jet notebook
 * Summary: Minimum spanning forest of an undirected graph
 * Description: n vertices; e[i]=(u,v,weight), each undirected edge supplied once. Returns (totalCost,chosenEdgeIDs); IDs index the original e, which is unchanged. Disconnected input gives one tree per component: components=n-chosen.size(). Accepts negative weights, parallel edges and self-loops. T is the weight/sum type; prefer ll. Uses UF from UnionFind.h. Time $O(V+E\log(E+1))$, space $O(V+E)$.
 * Usage: vec<tuple<int,int,ll>> e{{0,1,4},{1,2,2},{0,2,9}};
 *    auto [cost,ids]=kruskal(3,e); // cost=6
 
 */
#pragma once
#include "../data-structures/UnionFind.h"

template<class T>
pair<T,vec<int>> kruskal(int n,
		const vec<tuple<int,int,T>>& e) {
	vec<int> ord(sz(e)), ids;
	iota(ord.begin(),ord.end(),0);
	sort(ord.begin(),ord.end(),[&](int a,int b) {
		return get<2>(e[a]) < get<2>(e[b]);
	});
	UF uf(n);
	T cost = 0;
	for (int i : ord) {
		auto [u,v,w] = e[i];
		if (uf.join(u,v)) {
			cost += w; ids.pb(i);
		}
	}
	return {cost,ids};
}
