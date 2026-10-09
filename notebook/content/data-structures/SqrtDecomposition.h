/**
 * Author: jet notebook
 * Summary: Point assignment and range counts of values <=x.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use for changes a[i]=x and queries asking
 *  how many values are <=x in a range. Construct from a;
 *  set(i,x) assigns; countLE(l,r,x) returns the count
 *  in $[l,r)$. a holds values, b holds sorted blocks.
 *  New values are allowed; T only needs ordering.
 *  \item $B\approx\sqrt n$: build $O(n\log B)$,
 *  set $O(B)$, query $O(B+(n/B)\log B)$; space $O(n)$.
 *  \end{itemize}
 * Usage: vec<ll> a{3,1,3}; SqrtDecomposition d(a);
 *  d.set(2,0); int k = d.countLE(0,3,2); // 2
 */
#pragma once

template<class T = ll> struct SqrtDecomposition {
	int n, len;
	vec<T> a;
	vec<vec<T>> b;
	SqrtDecomposition(const vec<T>& v = {})
		: n(sz(v)), len(max(1, int(sqrt(n)))), a(v),
		  b((n+len-1)/len) {
		rep(i,0,n)
			b[i/len].pb(a[i]);
		for (auto& v1 : b)
			sort(v1.begin(), v1.end());
	}
	void set(int i, T x) {
		assert(0 <= i && i < n);
		auto& v = b[i/len];
		v.erase(lower_bound(v.begin(), v.end(),
			a[i]));
		v.insert(lower_bound(v.begin(), v.end(),
			x), x);
		a[i] = x;
	}
	int countLE(int l, int r, const T& x) const {
		assert(0 <= l && l <= r && r <= n);
		int ans = 0;
		while (l < r) {
			if (l % len == 0 && r-l >= len) {
				const auto& v = b[l/len];
				auto it = upper_bound(v.begin(),
					v.end(), x);
				ans += int(it-v.begin());
				l += len;
			} else {
				ans += !(x < a[l]);
				++l;
			}
		}
		return ans;
	}
};
