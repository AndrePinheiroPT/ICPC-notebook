/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Modular product and fast exponentiation.
 * Description: modMul(a,b,m) expects normalized residues $0\le a,b<m$, m>0. modPow(b,e,m) accepts a signed negative base;
 *  e is nonnegative. Base and modulus have the same integer type, at most 64 bits. Returns a residue in $[0,m)$.
 *  Products use unsigned 128 bits. Product $O(1)$, power $O(\log(e+1))$, memory $O(1)$.
 * Usage: assert(modPow(2LL,10,7LL) == 2);
 */
#pragma once

using ull = unsigned long long;

template<class T> T modMul(T a, T b, T m) {
	static_assert(is_integral_v<T> && sizeof(T) <= 8);
	assert(m > 0 && a >= 0 && a < m && b >= 0 && b < m);
	return T((__uint128_t)a * b % m);
}
template<class T> T modPow(T b, ull e, T m) {
	assert(m > 0);
	T ans = 1 % m;
	b %= m;
	if (b < 0) b += m;
	while (e > 0) {
		if (e & 1) ans = modMul(ans,b,m);
		b = modMul(b,b,m);
		e /= 2;
	}
	return ans;
}
