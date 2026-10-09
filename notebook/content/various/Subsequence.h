/**
 * Author: jet notebook
 * Summary: Longest increasing / nondecreasing subsequence.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item lis(a) returns chosen INDICES in increasing order. lis(a,true)
 *  allows equal values. Indices need not be adjacent; values are a[id].
 *  Empty input gives empty output, tie-breaking arbitrary.
 *  \item With length k+1, tail[k] minimises the last value; par reconstructs
 *  predecessors. Input needs only <. $O(n\log n)$ time, $O(n)$ space.
 *  \end{itemize}
 * Usage: auto ids=lis(vec<int>{3,1,2,2,4}); // length 3
 */
#pragma once

template<class T> vec<int> lis(const vec<T>& a,
		bool nondecreasing = false) {
	vec<int> tail, par(sz(a),-1);
	rep(i,0,sz(a)) {
		int l = 0, r = sz(tail);
		while (l < r) {
			int m = l + (r-l)/2;
			bool extend = nondecreasing
				? !(a[i] < a[tail[m]]) : a[tail[m]] < a[i];
			if (extend) l = m+1;
			else r = m;
		}
		if (l > 0) par[i] = tail[l-1];
		if (l == sz(tail)) tail.pb(i);
		else tail[l] = i;
	}
	vec<int> ans;
	int v = tail.empty() ? -1 : tail.back();
	for (; v >= 0; v = par[v]) ans.pb(v);
	reverse(ans.begin(),ans.end());
	return ans;
}
