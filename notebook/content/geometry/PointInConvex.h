/**
 * Author: jet notebook
 * Summary: Point location in a convex polygon
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item -1 outside, 0 boundary, 1 inside.
 *  Strictly convex CCW hull; n=0/1/2 allowed.
 *  \item $O(\log(n+1))$ time, $O(1)$ space.
 *  Exact signs; follow Point's bounds.
 *  Reals may round near degeneracies.
 *  \end{itemize}
 * Usage: int where = pointInConvex(h,q);
 *  // h: CCW convex hull; q: query point
 */
#pragma once
#include "Predicates.h"

template<class T> int pointInConvex(
		const vec<Point<T>>& h, Point<T> q) {
	int n = sz(h);
	if (n < 3)
		return n && onSeg(h[0],h.back(),q) ? 0 : -1;
	auto a = h[0].cross(h[1],q);
	auto b = h[0].cross(h.back(),q);
	if (a < 0 || b > 0) return -1;
	if (a == 0) return onSeg(h[0],h[1],q) ? 0 : -1;
	if (b == 0)
		return onSeg(h[0],h.back(),q) ? 0 : -1;
	int l = 1, r = n-1;
	while (r-l > 1) {
		int m = (l+r)/2;
		if (h[0].cross(h[m],q) >= 0) l = m;
		else r = m;
	}
	return side(h[l],h[r],q);
}
