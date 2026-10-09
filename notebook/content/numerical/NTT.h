/**
 * Author: jet notebook
 * Summary: Exact convolution modulo 998244353.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use for polynomial products or counting
 *  pairs by sum. Set a[i],b[j] to coefficients/frequencies;
 *  conv(a,b) returns c with c[k]=sum of a[i]*b[k-i]
 *  modulo 998244353. Length is a.size()+b.size()-1;
 *  empty if either input is empty. Inputs are copied.
 *  \item Values must be in $[0,mod)$; output length
 *  at most $2^{23}$. conv pads to a power of two and calls
 *  ntt; ntt(a,false) transforms a in place, true reverses
 *  it. Only call ntt directly on a positive power-of-two
 *  length up to $2^{23}$. Root 3 is specific to this prime.
 *  \item Padded length N: $O(N\log N)$ time,
 *  $O(N)$ space; counts wrap modulo mod.
 *  \end{itemize}
 * Usage: auto c = NTT::conv(vec<int>{1,2},vec<int>{3,4});
 *  // c = {3,10,8}
 */
#pragma once
#include "ModPower.h"

namespace NTT {
constexpr int mod = 998244353;

void ntt(vec<int>& a, bool inv = false) {
	int n = sz(a);
	assert(n > 0 && !(n & (n-1)) && n <= (1<<23));
	for (int i = 1, j = 0; i < n; ++i) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) j ^= bit;
		j ^= bit;
		if (i < j) swap(a[i], a[j]);
	}
	int g = inv ? int(modPow(3, mod-2, mod)) : 3;
	for (int k = 1; k < n; k *= 2) {
		int z = int(modPow(g, (mod-1)/(2*k), mod));
		for (int i = 0; i < n; i += 2*k) {
			ll w = 1;
			for (int j = 0; j < k; ++j) {
				int u = a[i+j];
				int v = int(w*a[i+j+k] % mod);
				a[i+j] = u+v < mod ? u+v : u+v-mod;
				a[i+j+k] = u-v < 0 ? u-v+mod : u-v;
				w = w*z % mod;
			}
		}
	}
	if (inv) {
		ll r = modPow(n, mod-2, mod);
		for (int& x : a) x = int(x*r % mod);
	}
}

vec<int> conv(vec<int> a, vec<int> b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a)+sz(b)-1, n = 1;
	assert(s <= (1<<23));
	while (n < s) n *= 2;
	a.resize(n); b.resize(n);
	ntt(a); ntt(b);
	for (int i = 0; i < n; ++i)
		a[i] = int(ll(a[i])*b[i] % mod);
	ntt(a, true);
	a.resize(s);
	return a;
}
}
