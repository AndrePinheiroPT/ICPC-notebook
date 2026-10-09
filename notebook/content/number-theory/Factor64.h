/**
 * Author: jet notebook
 * Summary: Prime factorization of large unsigned 64-bit integers.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Split a large integer into prime factors when trial
 *  division is too slow. factor64(n), unsigned n>=1, returns a
 *  sorted vec with repeats; factor64(1) is empty.
 *  \item Call factor64, not rho64: rho64 needs a composite and
 *  finds just one divisor. It advances x/y, batches differences
 *  in q, then tries a gcd; a failed batch is retried step by step.
 *  \item Randomized Pollard--Rho/Brent: expected $O(n^{1/4})$
 *  per split, O(log n) factorization space.
 *  \end{itemize}
 * Usage: auto f = factor64(360); // {2,2,2,3,3,5}
 */
#pragma once
#include "Prime64.h"

ull rho64(ull n) {
	if (n % 2 == 0) return 2;
	if (n % 3 == 0) return 3;
	static mt19937_64 rng(
		chrono::steady_clock::now()
		.time_since_epoch().count());
	for (;;) {
		ull y = rng()%(n-1)+1, c = rng()%(n-1)+1;
		ull r = 1, g = 1, x = 0, ys = 0;
		auto f = [&](ull v) {
			ull z = modMul(v, v, n);
			return z < n-c ? z+c : z-(n-c);
		};
		while (g == 1) {
			x = y;
			for (ull i = 0; i < r; ++i) y = f(y);
			for (ull k = 0; k < r && g == 1;
					k += 128) {
				ys = y;
				ull q = 1;
				for (ull i = 0; i < min(128ULL,r-k);
						++i) {
					y = f(y);
					ull d = x > y ? x-y : y-x;
					q = modMul(q, d, n);
				}
				g = gcd(q, n);
			}
			r *= 2;
		}
		if (g == n) do {
			ys = f(ys);
			g = gcd(x > ys ? x-ys : ys-x, n);
		} while (g == 1);
		if (g < n) return g;
	}
}

vec<ull> factor64(ull n) {
	assert(n >= 1);
	if (n == 1) return {};
	if (isPrime64(n)) return {n};
	ull p = rho64(n);
	auto a = factor64(p), b = factor64(n/p);
	a.insert(a.end(), b.begin(), b.end());
	sort(a.begin(), a.end());
	return a;
}
