/**
 * Author: Andre notebook; executable team adaptation
 * Summary: Digit DP: count numbers by digit-sum remainder.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item digitSumCount(bound,d,mod) counts integers in [0,bound] whose
 *  digit sum is divisible by d, modulo mod. bound is a nonempty decimal
 *  string (may be huge); d/mod are positive ints. Leading zeros and 0
 *  itself are accepted. Subtract one modulo mod to exclude 0.
 *  \item dp[sum][smaller] counts prefixes: sum is digit sum modulo d;
 *  smaller means already below bound. Start dp[0][0]=1. Append digit
 *  and update (sum+digit)\%d and smaller. The next layer starts at zero.
 *  \item For other digit restrictions, change the state/transition and
 *  accepted final states. Add started if leading zeros must not count as
 *  real digits (e.g. adjacency restrictions). Range [L,R] is F(R)-F(L-1).
 *  $O(10d|bound|)$ time, $O(d)$ space; does not list numbers.
 *  \end{itemize}
 * Usage: int count=digitSumCount("20",3); // 7, including 0
 */
#pragma once

int digitSumCount(const string& bound, int d, int mod = 1000000007) {
	assert(!bound.empty() && d > 0 && mod > 0);
	vec<array<int,2>> dp(d); dp[0][0] = 1%mod;
	for (char ch : bound) {
		assert('0' <= ch && ch <= '9'); int limit = ch-'0';
		vec<array<int,2>> next(d);
		rep(s,0,d) rep(smaller,0,2) rep(digit,0,10) {
			if (!smaller && digit > limit) continue;
			int ns = int((ll(s)+digit)%d), nt = smaller || digit < limit;
			next[ns][nt] = int((next[ns][nt]+ll(dp[s][smaller]))%mod);
		}
		dp.swap(next);
	}
	return int((ll(dp[0][0])+dp[0][1])%mod);
}
