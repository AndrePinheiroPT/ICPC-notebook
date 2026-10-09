/**
 * Author: team notebook
 * Summary: Longest valid sliding window.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item longestKDistinct(a,k) returns the first longest $[l,r)$ containing
 *  at most k distinct values; k>=0. Frequencies use map: $O(n\log(D+1))$
 *  time and $O(D)$ space, D the most distinct values stored.
 *  \item longestSumAtMost(a,limit) returns the first longest $[l,r)$ of sum
 *  at most limit. Requires nonnegative values, limit>=0 and total sum
 *  fitting ll. $O(n)$ time, $O(1)$ extra space. Negatives break the rule.
 *  \item Empty answer is [0,0). Expand the right endpoint; remove left values
 *  until valid. Both endpoints only move forwards.
 *  \end{itemize}
 * Usage: auto [l,r]=longestKDistinct(vec<int>{1,2,1,3},2); // [0,3)
 */
#pragma once

template<class T>
pair<int,int> longestKDistinct(const vec<T>& a, int k) {
	assert(k >= 0);
	map<T,int> freq;
	pair<int,int> best{0,0};
	for (int l = 0, r = 0; r < sz(a); ) {
		++freq[a[r++]];
		while (sz(freq) > k) {
			auto it = freq.find(a[l++]);
			if (--it->second == 0) freq.erase(it);
		}
		if (r-l > best.second-best.first) best = {l,r};
	}
	return best;
}
pair<int,int> longestSumAtMost(const vec<ll>& a, ll limit) {
	assert(limit >= 0);
	ll sum = 0;
	pair<int,int> best{0,0};
	for (int l = 0, r = 0; r < sz(a); ) {
		assert(a[r] >= 0);
		sum += a[r++];
		while (sum > limit) sum -= a[l++];
		if (r-l > best.second-best.first) best = {l,r};
	}
	return best;
}
