/**
 * Author: Simon Lindholm; team adaptation of Andre notebook
 * License: CC0
 * Summary: Faster partition-DP layer with monotone optimal split points.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item partitionLayer(prev,cost,INF) returns cur of size n+1, where
 *  cur[j]=min over 0<=k<j of prev[k]+cost(k,j). cost(k,j) is the cost
 *  of nonempty interval $[k,j)$; cur[0]=INF (one more nonempty group).
 *  Initialise prev[0]=0, other entries INF; call once per group.
 *  \item REQUIRES the leftmost optimal split not to decrease as j grows.
 *  Prove this for the cost (often via a Monge inequality); not valid for
 *  arbitrary DP. rec computes the midpoint, then restricts candidate
 *  splits for each half. Unreachable prev entries are skipped.
 *  \item INF is a positive sentinel; every finite value/intermediate
 *  must lie strictly between -INF and INF. Cost evaluation must be O(1)
 *  for $O(n\log n)$ per layer; $O(n)$ space. No reconstruction.
 *  \end{itemize}
 * Usage: vec<ll> prev(n+1,INFLL); prev[0]=0;
 *  // Squared sums: monotone split points when array values are nonnegative.
 *  auto cur=partitionLayer(prev,[&](int l,int r) {
 *    ll s=prefix[r]-prefix[l]; return s*s;
 *  },INFLL);
 */
#pragma once

template<class F>
vec<ll> partitionLayer(const vec<ll>& prev, F cost, ll inf = INFLL) {
	assert(!prev.empty() && inf > 0);
	int n = sz(prev)-1;
	vec<ll> cur(n+1,inf);
	auto rec = [&](auto&& self, int l, int r, int lo, int hi) -> void {
		if (l >= r) return;
		int mid = l+(r-l)/2, best = lo;
		rep(k,lo,min(mid,hi)) if (prev[k] != inf) {
			__int128 value = (__int128)prev[k]+cost(k,mid);
			assert(-(__int128)inf < value && value < inf);
			if (value < cur[mid]) { cur[mid] = ll(value); best = k; }
		}
		self(self,l,mid,lo,best+1);
		self(self,mid+1,r,best,hi);
	};
	rec(rec,1,n+1,0,n);
	return cur;
}
