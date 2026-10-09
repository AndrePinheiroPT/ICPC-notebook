/**
 * Author: zyndek; adapted for the notebook
 * Summary: Reachable sums using each item at most once.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item subsetBits<N>(weights) returns bits for achievable sums $[0,N)$,
 *  including 0. N>0 is a compile-time bound; weights are nonnegative
 *  int/ll. Weights >=N can be ignored. Does not reconstruct items or count ways.
 *  \item For weight x, dp<<x adds x to old sums, OR keeps the option to skip.
 *  $O(n\lceil N/B\rceil)$ time, $O(\lceil N/B\rceil)$ words (B bits/word).
 *  \end{itemize}
 * Usage: auto possible=subsetBits<11>(vec<int>{3,7}); // possible[10]=1
 */
#pragma once

template<size_t N, class T>
bitset<N> subsetBits(const vec<T>& w) {
	static_assert(N > 0);
	bitset<N> dp;
	dp[0] = 1;
	for (T x : w) {
		assert(x >= 0);
		if (uintmax_t(x) < N) dp |= dp << size_t(x);
	}
	return dp;
}
