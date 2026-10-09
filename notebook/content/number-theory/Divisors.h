/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Enumerate all positive divisors from a prime factorization.
 * Description: Pass SORTED prime factors with repetition, not n itself; their product must fit T.
 *  Returns divisors in unspecified order; empty factors (n=1) return \{1\}.
 *  For each distinct prime, extends previous divisors by all its powers.
 *  O(tau(n)) time and output size.
 * Usage: auto d = divisors(vec<ll>{2,2,3});
 *  sort(all(d)); // {1,2,3,4,6,12}
 */
#pragma once

template<class T>
vec<T> divisors(const vec<T>& f) {
	vec<T> d{1};
	for (int i = 0; i < sz(f);) {
		int k = sz(d), j = i;
		T power = 1;
		while (j < sz(f) && f[j] == f[i]) {
			power *= f[i];
			for (int q = 0; q < k; ++q)
				d.pb(d[q]*power);
			++j;
		}
		i = j;
	}
	return d;
}
