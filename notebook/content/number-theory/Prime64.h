/**
 * Author: jet notebook
 * Summary: Deterministic primality testing over the full unsigned 64-bit range.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Ask whether one large integer is prime.
 *  isPrime64(n) returns bool for any unsigned 64-bit n;
 *  0 and 1 return false. It does not return factors.
 *  \item Miller--Rabin: write $n-1=d\cdot2^s$, then test seven bases.
 *  These bases make the answer exact throughout the 64-bit range.
 *  O(log n) modular products, O(1) space.
 *  \end{itemize}
 * Usage: bool prime = isPrime64(1000000007ULL);
 */
#pragma once
#include "../numerical/ModPower.h"

bool isPrime64(ull n) {
	if (n < 2 || n % 2 == 0) return n == 2;
	int s = 0;
	ull d = n-1;
	while (d % 2 == 0) { d /= 2; ++s; }
	for (ull a : {2ULL, 325ULL, 9375ULL, 28178ULL,
		450775ULL, 9780504ULL, 1795265022ULL}) {
		if (a % n == 0) continue;
		ull x = modPow(a%n, d, n);
		if (x == 1 || x == n-1) continue;
		bool passed = false;
		for (int j = 1; j < s; ++j) {
			x = modMul(x, x, n);
			if (x == n-1) { passed = true; break; }
		}
		if (!passed) return false;
	}
	return true;
}
