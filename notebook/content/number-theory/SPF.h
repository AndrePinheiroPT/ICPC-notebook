/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Smallest-prime-factor sieve for many factorizations.
 * Description: SPF(lim) covers [0,lim), lim>=0. p[x] is the least prime factor;
 *  for x>=2, p[x]==x means prime; p[0]=0 and p[1]=1 when present.
 *  factor(x), 1<=x<lim, returns sorted prime factors with repetition; factor(1) is empty.
 *  Build O(lim log log lim) time, O(lim) memory; query O(log x).
 * Usage: SPF s(101); auto f = s.factor(60); // {2,2,3,5}
 */
#pragma once

struct SPF {
	vec<int> p;
	SPF(int lim) : p(lim) {
		iota(p.begin(), p.end(), 0);
		for (int i = 2; i <= (lim-1)/i; ++i)
			if (p[i] == i)
				for (ll j = ll(i)*i; j < lim; j += i)
					if (p[j] == j) p[j] = i;
	}
	vec<int> factor(int x) const {
		assert(1 <= x && x < sz(p));
		vec<int> f;
		while (x > 1) {
			f.pb(p[x]);
			x /= p[x];
		}
		return f;
	}
};
