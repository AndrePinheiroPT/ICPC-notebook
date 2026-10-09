/**
 * Author: jet notebook
 * Summary: Clip a polygon by a half-plane
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Keep the LEFT of $a\to b$, inclusive.
 *  Convex polygon; preserves vertex order.
 *  Empty/point/segment inputs/outputs allowed.
 *  \item $a\ne b$; no repeated closing vertex.
 *  Exact signs (eps=0); real arithmetic rounds.
 *  $O(n)$ time and extra space.
 *  \end{itemize}
 * Usage: auto q = convexCut(p,a,b);
 *  // p: polygon vertices; q: part left of a->b
 */
#pragma once
#include "Predicates.h"

vec<P2> convexCut(const vec<P2>& p,
		P2 a, P2 b) {
	assert(!(a == b));
	vec<P2> out;
	auto add = [&](P2 q) {
		if (out.empty() || !(out.back() == q))
			out.pb(q);
	};
	int n = sz(p);
	rep(i,0,n) {
		P2 u = p[i], v = p[(i+1)%n];
		ld s = a.cross(b,u), t = a.cross(b,v);
		if (s >= 0) add(u);
		if (n == 2 && i == 1) continue;
		if ((s < 0 && t > 0) || (s > 0 && t < 0))
			add(u+(v-u)*(s/(s-t)));
	}
	if (sz(out) > 1 && out[0] == out.back())
		out.pop_back();
	return out;
}
