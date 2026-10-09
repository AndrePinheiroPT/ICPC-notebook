/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Multiply modular matrices or raise a transition matrix to a power.
 * Description: Square matrices with entries in $[0,mod)$; matMul(a,b,mod) returns ab and matPow(a,e,mod) returns $a^e$.
 *  e>=0, integer mod>=1; e=0 gives identity. State as a column: next[i]=sum A[i][j]*state[j].
 *  128-bit products support integer types through 64 bits. For real arithmetic remove reductions and use long double.
 *  Product $O(n^3)$; power $O(n^3\log(e+1))$; $O(n^2)$ memory.
 * Usage: vec<vec<ll>> a{{1,1},{1,0}};
 *  auto f = matPow(a,10,1000000007LL); // f[0][1]=55
 */
#pragma once

template<class T>
vec<vec<T>> matMul(const vec<vec<T>>& a,
		const vec<vec<T>>& b, T mod) {
	static_assert(is_integral_v<T> && sizeof(T) <= 8);
	assert(mod >= 1 && sz(a) == sz(b));
	int n = sz(a);
	for (int i = 0; i < n; ++i) {
		assert(sz(a[i]) == n && sz(b[i]) == n);
		for (int j = 0; j < n; ++j) {
			assert(a[i][j] >= 0 && a[i][j] < mod);
			assert(b[i][j] >= 0 && b[i][j] < mod);
		}
	}
	vec<vec<T>> c(n,vec<T>(n));
	for (int i = 0; i < n; ++i)
		for (int k = 0; k < n; ++k)
			for (int j = 0; j < n; ++j)
				c[i][j] = T((c[i][j] + (__uint128_t)
					a[i][k]*b[k][j]) % mod);
	return c;
}
template<class T>
vec<vec<T>> matPow(vec<vec<T>> a,
		ll e, T mod) {
	assert(e >= 0 && mod >= 1);
	int n = sz(a);
	for (const auto& row : a) {
		assert(sz(row) == n);
		for (T x : row) assert(x >= 0 && x < mod);
	}
	vec<vec<T>> r(n,vec<T>(n));
	for (int i = 0; i < n; ++i) r[i][i] = T(1)%mod;
	while (e) {
		if (e&1) r = matMul(r,a,mod);
		e /= 2;
		if (e) a = matMul(a,a,mod);
	}
	return r;
}
