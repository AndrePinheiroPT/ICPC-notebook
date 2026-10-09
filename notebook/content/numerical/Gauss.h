/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Solve real linear equations; return rank and one solution.
 * Description: Pass augmented rows a[i]=\{coefficients...,rhs\}, variable count m, output x and absolute tolerance eps.
 *  Modifies a. Returns -1 if inconsistent, m for a unique solution, otherwise rank with free variables zero.
 *  x has m entries. Partial pivoting reduces numerical error; finite inputs/intermediates required and scaling matters.
 *  $O(nm\min(n,m))$ time, $O(m)$ extra memory.
 * Usage: vec<vec<double>> a{{2,1,5},{1,-1,1}}; vec<double> x;
 *  int rank = gauss(a,2,x); // rank=2, x={2,1}
 */
#pragma once

template<class T>
int gauss(vec<vec<T>>& a, int m,
		vec<T>& x, T eps = T(1e-12L)) {
	static_assert(is_floating_point_v<T>);
	assert(m >= 0 && eps > 0);
	int n = sz(a), rank = 0;
	vec<int> col;
	x.assign(m, T{});
	for (auto& row : a) assert(sz(row) == m+1);
	for (int c = 0; c < m && rank < n; ++c) {
		int pivot = rank;
		for (int i = rank+1; i < n; ++i)
			if (abs(a[i][c]) > abs(a[pivot][c])) pivot = i;
		if (abs(a[pivot][c]) <= eps) continue;
		swap(a[rank], a[pivot]);
		for (int i = rank+1; i < n; ++i) {
			T factor = a[i][c] / a[rank][c];
			a[i][c] = 0;
			for (int j = c+1; j <= m; ++j)
				a[i][j] -= factor * a[rank][j];
		}
		col.pb(c);
		++rank;
	}
	for (int i = rank; i < n; ++i)
		if (abs(a[i][m]) > eps) return -1;
	for (int i = rank-1; i >= 0; --i) {
		int c = col[i];
		T value = a[i][m];
		for (int j = c+1; j < m; ++j)
			value -= a[i][j] * x[j];
		x[c] = value / a[i][c];
	}
	return rank;
}
