/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Factor a small integer without preprocessing.
 * Description: trialDivision(n), integer n>=1, returns sorted prime factors with repetition.
 *  n=1 returns empty. O(sqrt(n)) time, O(log n) output size; slow for large primes.
 *  Use SPF for many bounded queries or factor64 for large 64-bit integers.
 * Usage: auto f = trialDivision(84LL); // {2,2,3,7}
 */
#pragma once

template<class T> vec<T> trialDivision(T n) {
	assert(n >= 1);
	vec<T> f;
	for (T p = 2; p <= n/p; ++p) {
		while (n % p == 0) {
			f.pb(p);
			n /= p;
		}
	}
	if (n > 1) f.pb(n);
	return f;
}
