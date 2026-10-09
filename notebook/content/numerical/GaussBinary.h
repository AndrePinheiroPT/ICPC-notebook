/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Solve XOR/parity equations over bits.
 * Description: a[i][j] indicates whether equation i uses variable j; a[i][m] is its RHS bit.
 *  Choose bitset<K> with K>m. Pass a,m,x; modifies a and fills x with m bits.
 *  Returns -1 if impossible, m if unique, otherwise rank with free variables zero.
 *  XOR replaces subtraction. $O(nm\lceil K/w\rceil)$ time and $O(m+K/w)$ extra memory.
 * Usage: vec<bitset<4>> a{bitset<4>(5),bitset<4>(6)}; vec<int> x;
 *  int rank = gaussBinary(a,2,x); // x={1,1}
 */
#pragma once

template<size_t K>
int gaussBinary(vec<bitset<K>>& a, int m, vec<int>& x) {
	assert(0 <= m && size_t(m) < K);
	int n = sz(a), rank = 0;
	vec<int> col;
	x.assign(m,0);
	for (int c = 0; c < m && rank < n; ++c) {
		int pivot = rank;
		while (pivot < n && !a[pivot][c]) ++pivot;
		if (pivot == n) continue;
		swap(a[pivot],a[rank]);
		for (int i = 0; i < n; ++i)
			if (i != rank && a[i][c]) a[i] ^= a[rank];
		col.pb(c); ++rank;
	}
	for (int i = rank; i < n; ++i)
		if (a[i][m]) return -1;
	for (int i = 0; i < rank; ++i) x[col[i]] = a[i][m];
	return rank;
}
