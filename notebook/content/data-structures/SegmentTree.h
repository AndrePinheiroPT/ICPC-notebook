/**
 * Author: Lucian Bicsi; team adaptation
 * License: CC0
 * Summary: Point assignment and range aggregate (maximum by default).
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item SegTree(n) starts with n neutral values. update(i,x) assigns
 *  a[i]=x; query(l,r) returns max on $[l,r)$, unit for an empty range.
 *  T is ll. To adapt, change T, merge and unit together: merge must be
 *  associative and unit its identity (sum/0, min/LLONG\_MAX, etc.).
 *  \item Left/right accumulators preserve order for noncommutative
 *  operations. s stores partial results. Build/space $O(n)$;
 *  update/query $O(\log n)$. Arithmetic must fit T.
 *  \end{itemize}
 * Usage: SegTree st(3); st.update(1,7); ll x=st.query(0,2); // 7
 */
#pragma once

struct SegTree {
	using T = ll;
	static constexpr T unit = LLONG_MIN;
	static T merge(T a, T b) { return max(a,b); }
	vec<T> s; int n;
	SegTree(int m = 0) : s(2*m,unit), n(m) { assert(m >= 0); }
	void update(int i, T x) {
		assert(0 <= i && i < n);
		s[i += n] = x;
		for (i /= 2; i; i /= 2)
			s[i] = merge(s[2*i],s[2*i+1]);
	}
	T query(int l, int r) const {
		assert(0 <= l && l <= r && r <= n);
		T left = unit, right = unit;
		for (l += n, r += n; l < r; l /= 2, r /= 2) {
			if (l & 1) left = merge(left,s[l++]);
			if (r & 1) right = merge(s[--r],right);
		}
		return merge(left,right);
	}
};
