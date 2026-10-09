/**
 * Author: team notebook
 * Summary: Minimum of every fixed-size sliding window.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item slidingMin(a,k), k>0, returns indices: ans[l] locates a minimum in
 *  $[l,l+k)$. Empty if k>n; ties choose the rightmost. Use a[ans[l]]
 *  for the value. Change >= to <= for sliding maxima.
 *  \item q stores candidates: expire old indices at the front, remove dominated
 *  ones at the back. $O(n)$ time, $O(\min(n,k))$ space plus output.
 *  \end{itemize}
 * Usage: auto at=slidingMin(vec<int>{4,2,5,1,3},3); // {1,3,3}
 */
#pragma once

template<class T>
vec<int> slidingMin(const vec<T>& a, int k) {
	assert(k > 0);
	deque<int> q;
	vec<int> ans;
	rep(r,0,sz(a)) {
		while (!q.empty() && q.front() <= r-k)
			q.pop_front();
		while (!q.empty() && a[q.back()] >= a[r])
			q.pop_back();
		q.pb(r);
		if (r >= k-1) ans.pb(q.front());
	}
	return ans;
}
