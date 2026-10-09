/**
 * Author: jet notebook
 * Summary: Orientation and segment membership
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item side: -1 right, 0 collinear, 1 left.
 *  onSeg includes endpoints, including a=b.
 *  toReal converts to Point<long double>.
 *  \item $O(1)$; all arithmetic must fit T.
 *  eps $\ge0$ is in squared-length units; default 0.
 *  Keep sorting exact; real predicates may round.
 *  \end{itemize}
 * Usage: int s = side(a,b,p); // side of line a->b
 *  bool hit = onSeg(a,b,p); // segment [a,b]
 *  P2 q = toReal(p); // convert coordinates
 */
#pragma once
#include "Point.h"

using ld = long double;
using P2 = Point<ld>;

template<class T> P2 toReal(Point<T> p) {
	return P2(ld(p.x),ld(p.y));
}
template<class T> int geoSgn(T x, T eps=T{}) {
	return (x > eps)-(x < -eps);
}
template<class T>
int side(Point<T> a, Point<T> b, Point<T> p,
		T eps=0) {
	return geoSgn(a.cross(b,p),eps);
}
template<class T>
bool onSeg(Point<T> a, Point<T> b, Point<T> p,
		T eps=0) {
	return !side(a,b,p,eps) &&
		(p-a).dot(p-b) <= eps;
}
