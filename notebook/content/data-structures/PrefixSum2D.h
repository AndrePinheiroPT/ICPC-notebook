/**
 * Author: jet notebook
 * Summary: Static rectangle sums.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item PrefixSum2D(rows,cols,a) receives a rectangular matrix with those
 *  dimensions. query(r1,c1,r2,c2) sums rows $[r_1,r_2)$ and columns
 *  $[c_1,c_2)$; an empty rectangle gives zero. p has an extra zero row/column.
 *  \item Build/space $O((n+1)(m+1))$, query $O(1)$; intermediate sums fit T.
 *  \end{itemize}
 * Usage: PrefixSum2D p(2,2,vec<vec<ll>>{{1,2},{3,4}});
 *  ll x=p.query(0,1,2,2); // 6
 */
#pragma once

template<class T = ll> struct PrefixSum2D {
	int n, m;
	vec<vec<T>> p;
	PrefixSum2D(int rows, int cols,
		const vec<vec<T>>& a)
		: n(rows), m(cols), p(n+1, vec<T>(m+1)) {
		assert(n >= 0 && m >= 0 && sz(a) == n);
		rep(r,0,n) {
			assert(sz(a[r]) == m);
			rep(c,0,m)
				p[r+1][c+1] = a[r][c] + p[r][c+1]
					+ p[r+1][c] - p[r][c];
		}
	}
	T query(int r1, int c1, int r2, int c2) const {
		assert(0 <= r1 && r1 <= r2 && r2 <= n);
		assert(0 <= c1 && c1 <= c2 && c2 <= m);
		return p[r2][c2] - p[r1][c2]
			- p[r2][c1] + p[r1][c1];
	}
};
