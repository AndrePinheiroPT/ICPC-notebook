/**
 * Author: team notebook
 * Summary: Static range sums and offline range additions.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item prefixSums(a) returns n+1 entries, p[0]=0; rangeSum(p,l,r) returns
 *  the sum on $[l,r)$. Negative values allowed; all sums must fit ll.
 *  \item Start a difference array d(n+1,0). addRange(d,l,r,x) adds x on
 *  $[l,r)$; finishDiff(d) returns the n final values without changing d.
 *  Add the initial array afterwards, if needed; no intermediate queries.
 *  \item Prefix construction/finalisation $O(n)$, query/add $O(1)$; space $O(n)$.
 *  \end{itemize}
 * Usage: auto p = prefixSums(vec<int>{4,-2,7});
 *  ll x = rangeSum(p,1,3); // 5
 */
#pragma once

template<class T> vec<ll> prefixSums(const vec<T>& a) {
	vec<ll> p(sz(a)+1);
	rep(i,0,sz(a)) p[i+1] = p[i]+ll(a[i]);
	return p;
}
ll rangeSum(const vec<ll>& p, int l, int r) {
	assert(0 <= l && l <= r && r < sz(p));
	return p[r]-p[l];
}
void addRange(vec<ll>& d, int l, int r, ll x) {
	assert(0 <= l && l <= r && r < sz(d));
	if (l == r) return;
	d[l] += x; d[r] -= x;
}
vec<ll> finishDiff(const vec<ll>& d) {
	assert(!d.empty());
	vec<ll> a(sz(d)-1);
	ll value = 0;
	rep(i,0,sz(a)) a[i] = value += d[i];
	return a;
}
