/**
 * Author: team notebook
 * Summary: Nearest strictly smaller neighbour.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item nearestSmaller(a) returns the index of the nearest STRICTLY smaller
 *  value to the left (-1 if absent). With right=true, searches right
 *  (absent=n). Returns boundaries/indices, not values.
 *  \item Stack values increase: pop >= removes ties. To accept <= neighbours,
 *  change pop to >; reverse comparisons for greater neighbours.
 *  Counting subarray contributions with duplicates needs one strict
 *  boundary and one non-strict boundary. $O(n)$ time/space.
 *  \end{itemize}
 * Usage: auto left=nearestSmaller(vec<int>{3,1,2,2}); // {-1,-1,1,1}
 */
#pragma once

template<class T>
vec<int> nearestSmaller(const vec<T>& a, bool right = false) {
	int n = sz(a);
	vec<int> ans(n,right ? n : -1), st;
	rep(t,0,n) {
		int i = right ? n-1-t : t;
		while (!st.empty() && a[st.back()] >= a[i])
			st.pop_back();
		if (!st.empty()) ans[i] = st.back();
		st.pb(i);
	}
	return ans;
}
