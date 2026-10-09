/**
 * Author: jet notebook
 * Summary: Intersection of two closed segments
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *   \item Segment intersection: 0/1 points, or 2
 *   endpoints of overlap, lexicographically sorted.
 *   Degenerate segments, touching and overlap valid.
 *   \item $O(1)$ time/space; integer predicates exact
 *   within Point bounds. Output is approximate P2;
 *   real predicates compare with zero.
 *   \end{itemize}
 * Usage: auto pts = segInter(a,b,c,d);
 *  // endpoints a,b and c,d; intersection points
 */
#pragma once
#include "Predicates.h"
#include "lineIntersection.h"

template<class T>
vec<P2> segInter(Point<T> a, Point<T> b,
		Point<T> c, Point<T> d) {
	if (side(a,b,c)*side(a,b,d) < 0 &&
			side(c,d,a)*side(c,d,b) < 0) {
		P2 A=toReal(a), B=toReal(b);
		P2 C=toReal(c), D=toReal(d);
		if ((B-A).dist2() > (D-C).dist2()) {
			swap(A,C); swap(B,D);
		}
		auto hit = lineInter(P2(),B-A,C-A,D-A);
		return {A+hit.second};
	}
	vec<Point<T>> q;
	for (auto p : {a,b})
		if (onSeg(c,d,p)) q.pb(p);
	for (auto p : {c,d})
		if (onSeg(a,b,p)) q.pb(p);
	sort(q.begin(),q.end());
	q.erase(unique(q.begin(),q.end()),q.end());
	vec<P2> ans;
	for (auto p : q) ans.pb(toReal(p));
	return ans;
}
