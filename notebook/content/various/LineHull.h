/**
 * Author: Simon Lindholm
 * License: CC0
 * Summary: Dynamic convex hull trick: maximum over inserted lines.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item DP asking for max(k[j]*x+m[j]) over earlier choices.
 *  Call add(k,m) to insert a line, query(x) to get the maximum y.
 *  ll inputs/result; slopes may arrive in any order, duplicates allowed.
 *  \item query needs at least one line. Internally lines are ordered
 *  by slope; p is the last integer x where that line wins.
 *  updateEnd updates it and flags a redundant next line.
 *  Breakpoints and arithmetic use \_\_int128; returned y must fit ll.
 *  \item Add amortized O(log n), query O(log n), O(n) space; no removals.
 *  \end{itemize}
 * Usage: LineHull h; h.add(2,1); h.add(-1,3);
 *  ll best = h.query(4); // 9
 */
#pragma once


struct HullLine {
	ll k, m; mutable __int128 p;
	bool operator<(const HullLine& o) const {
		return k < o.k;
	}
	bool operator<(ll x) const {return p < x;}
};

struct LineHull : multiset<HullLine,less<>> {
	static constexpr __int128 inf = (__int128)1 << 126;
	static __int128 floor(__int128 a, __int128 b) {
		return a/b - ((a%b != 0) && ((a<0) != (b<0)));
	}
	bool updateEnd(iterator x, iterator y) {
		if (y == end()) {
			x->p = inf; return false;
		}
		if (x->k == y->k)
			x->p = x->m > y->m ? inf : -inf;
		else x->p = floor((__int128)y->m-x->m,(__int128)x->k-y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto y = insert({k,m,0});
		auto z = next(y), x = y;
		// Remove redundant lines on the right.
		while (updateEnd(y,z)) z = erase(z);
		if (x != begin()) {
			--x;
			if (updateEnd(x,y)) {
				y = erase(y); // New line is redundant.
				updateEnd(x,y);
			}
		}
		while (x != begin()) { // Then prune the left.
			y = x; --x;
			if (x->p < y->p) break;
			y = erase(y);
			updateEnd(x,y);
		}
	}
	ll query(ll x) const {
		assert(!empty());
		auto l = *lower_bound(x);
		__int128 ans = (__int128)l.k*x+l.m;
		assert(LLONG_MIN <= ans && ans <= LLONG_MAX);
		return ll(ans);
	}
};
