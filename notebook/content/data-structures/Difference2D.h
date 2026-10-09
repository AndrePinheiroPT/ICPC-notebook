/**
 * Author: jet notebook
 * Summary: Offline rectangle additions.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Difference2D<T>(n,m) starts with a zero matrix.
 *  add(r1,c1,r2,c2,x) adds x on $[r_1,r_2)\times[c_1,c_2)$.
 *  values() returns the final matrix, without consuming the structure.
 *  \item Use when queries are needed only after all updates. d stores corner
 *  changes; values computes their 2D prefix sums. Add $O(1)$;
 *  values/space $O(n(m+1))$. Intermediate values must fit T.
 *  \end{itemize}
 * Usage: Difference2D d(2,3); d.add(0,1,2,3,5); auto a=d.values();
 */
#pragma once

template<class T = ll> struct Difference2D {
	int n, m;
	vec<vec<T>> d;
	Difference2D(int rows, int cols)
		: n(rows), m(cols), d(n) {
		assert(n >= 0 && m >= 0);
		for (auto& row : d) row.resize(m);
	}
	void add(int r1, int c1, int r2, int c2,
			T delta) {
		assert(0 <= r1 && r1 <= r2 && r2 <= n);
		assert(0 <= c1 && c1 <= c2 && c2 <= m);
		if (r1 == r2 || c1 == c2) return;
		d[r1][c1] += delta;
		if (r2 < n) d[r2][c1] -= delta;
		if (c2 < m) d[r1][c2] -= delta;
		if (r2 < n && c2 < m) d[r2][c2] += delta;
	}
	vec<vec<T>> values() const {
		auto a = d;
		rep(r,0,n)
			rep(c,0,m) {
				if (r) a[r][c] += a[r-1][c];
				if (c) a[r][c] += a[r][c-1];
				if (r && c) a[r][c] -= a[r-1][c-1];
			}
		return a;
	}
};
