/**
 * Author: jet notebook
 * Summary: Largest rectangle under a histogram, with variable widths.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item largestRectangle(h,w) receives integer heights h>=0 and widths w>0
 *  of equal length. Returns (area,l,r) covering bars $[l,r)$; empty or
 *  all-zero heights give (0,0,0), ties arbitrary.
 *  \item x stores n+1 prefix widths; the stack waits for a smaller bar to
 *  close each candidate. For unit widths, use w(n,1). After compression,
 *  widths are original coordinate differences. Sums/areas must fit ll.
 *  $O(n)$ time/space.
 *  \end{itemize}
 * Usage: auto [area,l,r]=largestRectangle(
 *   vec<int>{2,1,4,4},vec<int>(4,1));
 */
#pragma once

template<class H, class W>
tuple<ll,int,int> largestRectangle(
		const vec<H>& h, const vec<W>& w) {
	int n = sz(h), left = 0, right = 0;
	vec<ll> x(n+1);
	rep(i,0,n)
		x[i+1] = x[i]+ll(w[i]);
	vec<int> st;
	ll best = 0;
	for (int r = 0; r <= n; ++r) {
		while (!st.empty() &&
				(r == n || h[st.back()] >= h[r])) {
			int j = st.back(); st.pop_back();
			int l = st.empty() ? 0 : st.back()+1;
			ll area = ll(h[j])*(x[r]-x[l]);
			if (area > best) {
				best = area; left = l; right = r;
			}
		}
		if (r < n) st.pb(r);
	}
	return {best,left,right};
}
