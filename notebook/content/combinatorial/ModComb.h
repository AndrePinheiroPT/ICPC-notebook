/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Many binomial coefficients modulo one prime.
 * Description: Construct ModComb(lim,prime): $1\le lim\le prime$, PRIME modulus, $0\le n<lim$.
 *  choose(n,k) returns C(n,k) modulo p, or zero for k outside [0,n]. fact[i]=i!, invfact[i]=inverse factorial.
 *  The integer type is inferred from prime; products use 128 bits. Build $O(lim+\log p)$, memory $O(lim)$;
 *  each query $O(1)$. For n>=p use Lucas when p is small, not these factorials directly.
 * Usage: ModComb c(101,1000000007LL);
 *  assert(c.choose(100,3) == 161700);
 */
#pragma once
#include "../numerical/ModPower.h"

template<class T = ll> struct ModComb {
	T p;
	vec<T> fact, invfact;
	ModComb(int lim, T prime)
		: p(prime), fact(lim), invfact(lim) {
		assert(p >= 2 && 1 <= lim && T(lim) <= p);
		fact[0] = 1;
		for (int i = 1; i < lim; ++i)
			fact[i] = modMul(fact[i-1], T(i), p);
		invfact[lim-1] = modPow(fact[lim-1], ull(p-2), p);
		for (int i = lim-1; i > 0; --i)
			invfact[i-1] = modMul(invfact[i], T(i), p);
	}
	T choose(ll n, ll k) const {
		assert(0 <= n && n < sz(fact));
		if (k < 0 || k > n) return T(0);
		T ans = modMul(fact[n], invfact[k], p);
		return modMul(ans, invfact[n-k], p);
	}
};
