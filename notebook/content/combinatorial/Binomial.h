/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Exact number of unordered choices.
 * Description: binom(n,k) returns C(n,k), n>=0; k outside [0,n] gives zero.
 *  Result must fit T (ll by default); binom<cpp\_int> supports arbitrary precision.
 *  Cancels denominators before multiplying. O(min(k,n-k) log(n+1)) arithmetic time;
 *  big-integer operations cost more.
 * Usage: assert(binom(8,3) == 56);
 */
#pragma once

template<class T = ll> T binom(ll n, ll k) {
	assert(n >= 0);
	if (k < 0 || k > n) return T(0);
	k = min(k, n-k);
	T ans = 1;
	for (ll i = 1; i <= k; ++i) {
		ll a = n-k+i, b = i, g = gcd(a,b);
		a /= g; b /= g;
		ans = T(ans / b * a);
	}
	return ans;
}
