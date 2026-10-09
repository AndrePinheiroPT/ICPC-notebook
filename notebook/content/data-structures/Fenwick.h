/**
 * Author: jet, zyndek; adaptado ao notebook
 * Summary: Point additions, range sums and prefix selection.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Fenwick<T>(n) starts with n zeros (T defaults to ll). add(i,x)
 *  adds x; query(r) sums $[0,r)$; query(l,r) sums $[l,r)$.
 *  Indices are 0-based externally; t uses 1-based partial sums internally.
 *  \item lowerBound(s) returns the first i with query(i+1)>=s, or n if
 *  absent; returns -1 for s<=0. This search requires all array values
 *  nonnegative; ordinary sums allow negatives. All sums must fit T.
 *  \item Build/space $O(n)$; add/query/search $O(\log n)$.
 *  \end{itemize}
 * Usage: Fenwick f(3); f.add(1,7); int i=f.lowerBound(7); // 1
 */
#pragma once

template<class T = ll> struct Fenwick {
	int n;
	vec<T> t;
	Fenwick(int m = 0) : n(m), t(size_t(m)+1) {
		assert(n >= 0);
	}
	void add(int i, T delta) {
		assert(0 <= i && i < n);
		for (ll k = ll(i)+1; k <= n; k += k & -k)
			t[k] += delta;
	}
	T query(int r) const {
		assert(0 <= r && r <= n);
		T ans{};
		for (; r > 0; r -= r & -r) ans += t[r];
		return ans;
	}
	T query(int l, int r) const {
		assert(0 <= l && l <= r && r <= n);
		return query(r) - query(l);
	}
	int lowerBound(T sum) const {
		if (sum <= T(0)) return -1;
		int pos = 0, step = 1;
		while (step <= n/2) step *= 2;
		for (; step; step /= 2)
			if (step <= n-pos && t[pos+step] < sum) {
				pos += step;
				sum -= t[pos];
			}
		return pos;
	}
};
