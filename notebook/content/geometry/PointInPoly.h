/**
 * Author: jet notebook
 * Summary: Point location in a simple polygon
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item -1 outside, 0 boundary, 1 inside.
 *  Simple polygon, either orientation; n=0/1/2.
 *  \item $O(n)$ time, $O(1)$ space. Exact signs;
 *  follow Point's bounds; reals may round.
 *  \end{itemize}
 * Usage: int where = pointInPoly(p,q);
 *  // p: boundary-order vertices; q: query point
 */
#pragma once
#include "Predicates.h"

template<class T> int pointInPoly(
		const vec<Point<T>>& p, Point<T> q) {
	bool in = false;
	rep(i,0,sz(p)) {
		auto a = p[i], b = p[(i+1)%sz(p)];
		if (onSeg(a,b,q)) return 0;
		if ((a.y > q.y) != (b.y > q.y))
			if ((a.cross(b,q) > 0) == (b.y > a.y))
				in = !in;
	}
	return in ? 1 : -1;
}
