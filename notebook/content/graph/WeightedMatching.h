/**
 * Author: Jeremy Lim, Joshua Andersson
 * License: CC0
 * Summary: Minimum-cost assignment of every left vertex
 * Description: cost[n][m] is a complete bipartite cost matrix, n<=m. Returns (minimumTotalCost,match), with left u assigned to right match[u]; right vertices are used at most once. Negative costs allowed; negate for maximum cost. Missing edges need a sufficiently large finite penalty and a final check that none was chosen; use MinCostFlow for sparse constraints or partial assignments. Does not modify cost. dist/pot maintain reduced costs; rev is the reverse matching. Every intermediate difference/sum must fit ll. Empty left side returns cost 0 and an empty matching. Time $O(n^2m)$, extra space $O(n+m)$.
 * Usage: vec<vec<ll>> cost{{4,1,9},{2,8,3}};
 *   auto [total,match]=weightedMatching(cost);
 *   // total=3, match={1,0}
 
 */
#pragma once

pair<ll, vec<int>> weightedMatching(const vec<vec<ll>>& C) {
	int i = sz(C), m = i ? sz(C[0]) : 0, c, s, r;
	assert(i<=m);
	for (auto& row : C) assert(sz(row)==m);
	vec<ll> dist(m), pot(m);
	vec<int> match(i), rev(m, -1), cols(m), prev(m);
	ll d = 0, nd, cost = 0;
	while (i--) {
		rep(c,0,m) dist[c] = C[i][c], cols[c] = c, prev[c] = i;
		for (s = 0;;) {
			rep(j,s,m) {
				c = cols[j], nd = dist[c] - pot[c];
				if (j == s || d > nd) d = nd, swap(cols[s], cols[j]);
			}
			if ((r = rev[c = cols[s++]]) == -1) break;
			rep(j,0,m) if (dist[j] > (nd = C[r][j]-C[r][c]+dist[c]))
				dist[j] = nd, prev[j] = r;
		}
		cost += dist[c];
		while (s--) pot[cols[s]] = dist[cols[s]] - d;
		for (; r != i; swap(c, match[r]))
			r = rev[c] = prev[c];
	}
	return {cost, match};
}
