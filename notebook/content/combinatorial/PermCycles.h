/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Lengths of the disjoint cycles of a permutation.
 * Description: p must be a permutation of [0,n), mapping i to p[i]. Returns cycle lengths
 *  in unspecified order; fixed points have length 1. Empty input gives empty output.
 *  With c cycles, n-c is the minimum number of arbitrary swaps. O(n) time/memory.
 * Usage: auto len = cycleLengths({1,2,0,4,3}); // {3,2}
 */
#pragma once

vec<int> cycleLengths(const vec<int>& p) {
	vec<int> len, vis(sz(p));
	for (int i = 0; i < sz(p); ++i) if (!vis[i]) {
		int j = i, k = 0;
		do {
			vis[j] = 1; ++k;
			j = p[j];
		} while (j != i);
		len.pb(k);
	}
	return len;
}
