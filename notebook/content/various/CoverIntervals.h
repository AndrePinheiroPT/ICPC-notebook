/**
 * Author: jet notebook
 * Summary: Minimum number of intervals covering a target.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item coverIntervals(seg,L,R) receives intervals $[l,r)$ and target $[L,R)$.
 *  Returns original interval IDs in coverage order; nullopt if impossible,
 *  empty if L=R. Integers/reals allowed, l<=r and L<=R, no NaN.
 *  \item Greedily extend the current covered prefix as far as possible. Input
 *  is unchanged; $O(n\log n)$ time, $O(n)$ extra space. Closed integer
 *  intervals [a,b] become [a,b+1) if b+1 fits.
 *  \end{itemize}
 * Usage: auto ids=coverIntervals(vec<pair<int,int>>{{0,3},{2,5}},0,5);
 */
#pragma once

template<class T> optional<vec<int>> coverIntervals(
		const vec<pair<T,T>>& seg, T L, T R) {
	if (L == R) return vec<int>{};
	vec<int> ord(sz(seg)), ans;
	iota(ord.begin(),ord.end(),0);
	sort(ord.begin(),ord.end(),[&](int a,int b) {
		return seg[a] < seg[b];
	});
	int i = 0;
	while (L < R) {
		T reach = L;
		int id = -1;
		while (i < sz(ord) &&
				seg[ord[i]].first <= L) {
			int j = ord[i++];
			if (reach < seg[j].second) {
				reach = seg[j].second; id = j;
			}
		}
		if (id < 0) return nullopt;
		ans.pb(id); L = reach;
	}
	return ans;
}
