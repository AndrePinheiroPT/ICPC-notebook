/**
 * Author: Team reference, based on KACTL
 * Summary: Virtual tree connecting a small subset of tree vertices
 * Description: Preprocess the original tree with TreeLCA. compressTree(t,subset) adds LCAs and compresses paths with no selected branching point. Returns (parentIndex,originalVertex) for at most 2*|subset|-1 vertices in DFS order; parentIndex=-1 at the root, otherwise indexes this returned vector. Empty subset returns empty; duplicate IDs are ignored. A virtual edge stands for a whole original path: its unweighted length is dep[child]-dep[parent], or subtract weighted root-distances. Useful when many queries mention only a few vertices. Time $O(k\log k+k\log V)$, extra space $O(k)$, after LCA preprocessing.
 * Usage: TreeLCA t(g);
 *   auto vt=compressTree(t,{u,v});
 *   // vt[i].first is an index, vt[i].second an original ID.
 
 */
#pragma once
#include "TreeLCA.h"
vec<pair<int,int>> compressTree(const TreeLCA& t,
		vec<int> subset) {
	if (subset.empty()) return {};
	auto cmp=[&](int a,int b){return t.tin[a]<t.tin[b];};
	sort(all(subset),cmp);
	subset.erase(unique(all(subset)),subset.end());
	int k=sz(subset);
	rep(i,1,k) subset.pb(t.lca(subset[i-1],subset[i]));
	sort(all(subset),cmp);
	subset.erase(unique(all(subset)),subset.end());
	vec<pair<int,int>> ans;
	vec<int> st;
	for (int v : subset) {
		while (!st.empty() && t.tout[ans[st.back()].second]<=t.tin[v])
			st.pop_back();
		int p=st.empty() ? -1 : st.back();
		st.pb(sz(ans)); ans.pb({p,v});
	}
	return ans;
}
