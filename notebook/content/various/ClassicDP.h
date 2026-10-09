/**
 * Author: notebook contributors
 * Summary: Compact classical DP routines (values/counts, no reconstruction).
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item maxSubarray(a), nonempty ll array: returns (linear,circular) maximum
 *  NONEMPTY subarray sums; $O(n)$ time, $O(1)$ space. All sums fit ll.
 *  \item knapsack01(w,v,W): nonnegative integer weights, ll values, capacity
 *  W>=0. Each item at most once; returns maximum value with weight <=W,
 *  possibly choosing nothing. Descending capacities prevent reuse.
 *  $O(nW)$ time/$O(W)$ space; values/sums fit ll.
 *  \item minCoins(coins,S): positive unlimited coins, S>=0; minimum coin
 *  count or -1 if impossible. coinWays(coins,S,mod): DISTINCT positive
 *  coins; unordered combinations modulo positive mod. Coin loop outside
 *  avoids counting permutations. $O(|coins|S)$ time, $O(S)$ space.
 *  \item lcsLength(a,b): length of longest common SUBSEQUENCE of two strings,
 *  not necessarily contiguous; $O(nm)$ time, $O(m)$ space.
 *  \item gridPaths(g,mod): rectangular grid of . / \#, moves right/down from
 *  top-left to bottom-right. Returns count modulo positive mod; empty
 *  grid or blocked endpoint gives 0. $O(nm)$ time, $O(m)$ space.
 *  \end{itemize}
 * Usage: auto [linear,circular]=maxSubarray({5,-8,6}); // 6,11
 *  ll best=knapsack01({2,3,4},{4,5,7},5); // 9
 */
#pragma once

// Array não vazio -> {melhor soma linear, melhor soma circular}.
pair<ll,ll> maxSubarray(const vec<ll>& a) {
	assert(!a.empty());
	ll endMax = a[0], best = a[0], total = a[0];
	ll endMin = a[0], worst = a[0];
	rep(i,1,sz(a)) {
		endMax = max(a[i],endMax+a[i]);
		endMin = min(a[i],endMin+a[i]);
		best = max(best,endMax);
		worst = min(worst,endMin);
		total += a[i];
	}
	return {best,best < 0 ? best : max(best,total-worst)};
}

// Cada item uma vez; capacidade até W; pode não escolher nada.
ll knapsack01(const vec<int>& w, const vec<ll>& v, int W) {
	assert(sz(w) == sz(v) && 0 <= W && W < INT_MAX);
	vec<ll> dp(W+1);
	rep(i,0,sz(w)) {
		assert(w[i] >= 0);
		for (int x = W; x >= w[i]; --x)
			dp[x] = max(dp[x],dp[x-w[i]]+v[i]);
	}
	return dp[W];
}

// Moedas positivas ilimitadas -> mínimo, ou -1 se impossível.
int minCoins(const vec<int>& coins, int S) {
	assert(0 <= S && S < INT_MAX);
	for (int c : coins) assert(c > 0);
	vec<int> dp(S+1,S+1); dp[0] = 0;
	for (int s = 1; s <= S; ++s)
		for (int c : coins) if (c <= s && dp[s-c] < S+1)
			dp[s] = min(dp[s],dp[s-c]+1);
	return dp[S] == S+1 ? -1 : dp[S];
}

// Moedas positivas DISTINTAS ilimitadas -> combinações sem ordem.
int coinWays(const vec<int>& coins, int S, int mod = 1000000007) {
	assert(0 <= S && S < INT_MAX && mod > 0);
	vec<int> dp(S+1); dp[0] = 1%mod;
	for (int c : coins) {
		assert(c > 0);
		for (int s = c; s <= S; ++s)
			dp[s] = int((dp[s]+ll(dp[s-c]))%mod);
	}
	return dp[S];
}

// Pode saltar caracteres; devolve apenas o comprimento da LCS.
int lcsLength(const string& a, const string& b) {
	int n = sz(a), m = sz(b);
	vec<int> prev(m+1), cur(m+1);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j)
			cur[j] = a[i-1] == b[j-1] ? prev[j-1]+1
				: max(prev[j],cur[j-1]);
		swap(prev,cur);
	}
	return prev[m];
}

// Só baixo/direita; '.' livre, '#' bloqueado; devolve caminhos.
int gridPaths(const vec<string>& g, int mod = 1000000007) {
	assert(mod > 0);
	int n = sz(g), m = n ? sz(g[0]) : 0;
	for (const auto& row : g) assert(sz(row) == m);
	if (!n || !m) return 0;
	vec<int> dp(m); dp[0] = 1%mod;
	rep(r,0,n)
		rep(c,0,m)
			if (g[r][c] == '#') dp[c] = 0;
			else if (c) dp[c] = int((dp[c]+ll(dp[c-1]))%mod);
	return dp[m-1];
}
