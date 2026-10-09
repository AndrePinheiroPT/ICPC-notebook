/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Solve linear equations modulo a prime.
 * Description: Pass augmented rows a, number of variables m, output x, and PRIME p with $2\le p\le$ INT\_MAX.
 *  Copies and normalizes a, so negative coefficients are allowed. Returns -1 if inconsistent, m if unique,
 *  otherwise rank; x is one solution in $[0,p)$ with free variables zero. where[c] records each pivot row.
 *  $O(nm\min(n,m)+\min(n,m)\log p)$ time and $O(nm)$ copied memory. Not for composite moduli.
 * Usage: vec<vec<int>> a{{2,1,5},{1,-1,1}}; vec<int> x;
 *  int rank = gaussMod(a,2,x,7); // rank=2, x={2,1}
 */
#pragma once
#include "ModPower.h"

int gaussMod(vec<vec<int>> a, int m,
		vec<int>& x, int p) {
	assert(m >= 0 && p >= 2);
	int n = sz(a), rank = 0;
	vec<int> where(m,-1);
	x.assign(m, 0);
	for (auto& row : a) {
		assert(sz(row) == m+1);
		for (int& v : row) {
			v %= p;
			if (v < 0) v += p;
		}
	}
	for (int c = 0; c < m && rank < n; ++c) {
		int pivot = rank;
		while (pivot < n && !a[pivot][c]) ++pivot;
		if (pivot == n) continue;
		swap(a[rank], a[pivot]);
		where[c] = rank;
		ll inv = modPow(a[rank][c], p-2, p);
		for (int j = c; j <= m; ++j)
			a[rank][j] = int(a[rank][j] * inv % p);
		for (int i = 0; i < n; ++i) if (i != rank) {
			int factor = a[i][c];
			a[i][c] = 0;
			for (int j = c+1; j <= m; ++j)
				a[i][j] = int((a[i][j]
					- ll(factor)*a[rank][j]%p + p)%p);
		}
		++rank;
	}
	for (int i = rank; i < n; ++i)
		if (a[i][m]) return -1;
	for (int c = 0; c < m; ++c)
		if (where[c] != -1) x[c] = a[where[c]][m];
	return rank;
}
