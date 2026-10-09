/**
 * Author: jet notebook
 * Summary: Aggregate over all submasks or all supermasks.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item a has size $2^k>0$ and initially stores a value for each exact mask.
 *  sos(a) replaces a[mask] by the sum over its submasks; sos(a,true)
 *  uses supermasks. Includes the mask itself; modifies a, returns void.
 *  \item Optional op must be associative and commutative (default addition);
 *  all intermediate values fit T. small/big differ in one bit.
 *  $O(k2^k)$ operations, $O(1)$ extra space.
 *  \end{itemize}
 * Usage: vec<ll> a{1,2,3,4}; sos(a); // {1,3,4,10}
 */
#pragma once

template<class T, class F = plus<T>>
void sos(vec<T>& a, bool supersets = false,
		F op = {}) {
	int n = sz(a);
	assert(n > 0 && !(n & (n-1)));
	for (int bit = 1; bit < n; bit <<= 1) {
		rep(mask,0,n) {
			if (mask & bit) continue;
			int small = mask, big = mask | bit;
			if (supersets) a[small] = op(a[small],a[big]);
			else a[big] = op(a[big],a[small]);
		}
	}
}
