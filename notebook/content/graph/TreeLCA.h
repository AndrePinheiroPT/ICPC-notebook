/**
 * Author: jet notebook
 * Summary: Lowest common ancestor and tree distance
 * Description: Undirected connected tree g, chosen root. lca(a,b) returns the deepest common ancestor; dist(a,b) counts edges. ancestor(v,k) returns the k-th parent or -1 if absent. Inherits par/dep/tin/tout/ord from TreeTour; jt.jmp uses [level][vertex]. Build can be empty; queries require valid vertices. For weighted distances separately precompute root-distance D: D[a]+D[b]-2*D[lca(a,b)]. Build/space $O(V\log V)$; queries $O(\log V)$.
 * Usage: vec<vec<int>> g{{1},{0,2,3},{1},{1}};
 *    TreeLCA t(g);
 *    int c=t.lca(2,3); // 1
 *    int a=t.ancestor(2,3); // -1
 
 */
#pragma once
#include "TreeTour.h"
#include "JumpTable.h"

struct TreeLCA : TreeTour {
	JumpTable jt;
	static int levels(int n) {
		int d = 1;
		for (ll on = 1; on < n; on *= 2) ++d;
		return d;
	}
	TreeLCA(const vec<vec<int>>& g, int root = 0)
		: TreeTour(g,root), jt(par,levels(sz(g))) {}
	int ancestor(int v, ull k) const {
		return k > ull(dep[v]) ? -1 : jt.jump(v,k);
	}
	int lca(int a, int b) const {
		if (dep[a] < dep[b]) swap(a,b);
		a = jt.jump(a,ull(dep[a]-dep[b]));
		if (a == b) return a;
		for (int i = sz(jt.jmp); i--;) {
			int c = jt.jmp[i][a], d = jt.jmp[i][b];
			if (c != d) a = c, b = d;
		}
		return jt.jmp[0][a];
	}
	int dist(int a, int b) const {
		int c = lca(a,b);
		return (dep[a]-dep[c])+(dep[b]-dep[c]);
	}
};
