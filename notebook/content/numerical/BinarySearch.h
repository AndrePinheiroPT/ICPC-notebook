/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: First/last feasible integer in a half-open search range.
 * Description: firstTrue(l,r,check) requires false then true; lastTrue requires true then false.
 *  Only evaluates candidates in $[l,r)$. Returns optional answer; nullopt for no answer or empty range.
 *  The excluded endpoints act as conceptual sentinels, never evaluated. Midpoints use 128 bits,
 *  including searches spanning negative and positive ll values. $O(\log(r-l+1))$ predicate calls, $O(1)$ memory.
 * Usage: auto x = firstTrue(0,11,[](ll t) { return t/2+t/3 >= 5; });
 *  assert(x && *x == 6);
 */
#pragma once

template<class F>
optional<ll> firstTrue(ll l, ll r, F check) {
	assert(l <= r);
	ll end = r;
	while (l < r) {
		ll m = ll((__int128)l + ((__int128)r-l)/2);
		if (check(m)) r = m;
		else l = m+1;
	}
	if (l == end) return nullopt;
	return l;
}
template<class F>
optional<ll> lastTrue(ll l, ll r, F check) {
	assert(l <= r);
	ll begin = l;
	while (l < r) {
		ll m = ll((__int128)l + ((__int128)r-l)/2);
		if (check(m)) l = m+1;
		else r = m;
	}
	if (l == begin) return nullopt;
	return l-1;
}
