/**
 * Author: jet notebook
 * Summary: Projections and distances
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *   \item Project/reflect p on line ab; distances
 *   from p to line/segment ab are nonnegative.
 *   Lines require $a\ne b$; segments allow $a=b$.
 *   \item $O(1)$ time/space. Real arithmetic;
 *   intermediate values must be finite.
 *   \end{itemize}
 * Usage: P2 q = project(a,b,p), r = reflect(a,b,p);
 *  ld d = lineDist(a,b,p), e = segDist(a,b,p);
 *  // a,b define line/segment; p is query point
 */
#pragma once
#include "Predicates.h"

P2 project(P2 a, P2 b, P2 p) {
	P2 u = b-a;
	assert(u.dist2() > 0);
	return a+u*((p-a).dot(u)/u.dist2());
}
P2 reflect(P2 a, P2 b, P2 p) {
	return project(a,b,p)*2-p;
}
ld lineDist(P2 a, P2 b, P2 p) {
	assert(!(a == b));
	return abs((b-a).cross(p-a))/
		sqrtl((b-a).dist2());
}
ld segDist(P2 a, P2 b, P2 p) {
	if (a == b) return sqrtl((p-a).dist2());
	P2 u = b-a;
	ld t = clamp((p-a).dot(u)/u.dist2(),
		ld(0),ld(1));
	return sqrtl((p-(a+u*t)).dist2());
}
