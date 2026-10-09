/**
 * Author: jet notebook
 * Summary: Convex hull in counterclockwise order
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Monotone chain; unique vertices in CCW
 *  order, no collinear interiors/closing copy.
 *  Collinear input gives endpoints; n=0/1 valid.
 *  \item $O(n\log n)$ time, $O(n)$ space.
 *  Exact signs; follow Point's bounds.
 *  Reals may round near degeneracies.
 *  \end{itemize}
 * Usage: auto h = hull(p); // p: unordered input points
 */
#pragma once
#include "Predicates.h"

template<class T>
vec<Point<T>> hull(vec<Point<T>> p) {
	sort(p.begin(), p.end());
	p.erase(unique(p.begin(),p.end()),p.end());
	int n = sz(p), k = 0;
	if (n < 2) return p;
	vec<Point<T>> h(2*n);
	for (auto q : p) {
		while (k >= 2 &&
			h[k-2].cross(h[k-1],q) <= 0) --k;
		h[k++] = q;
	}
	for (int i = n-2, t = k+1; i >= 0; --i) {
		while (k >= t &&
			h[k-2].cross(h[k-1],p[i]) <= 0) --k;
		h[k++] = p[i];
	}
	h.resize(k-1);
	return h;
}
