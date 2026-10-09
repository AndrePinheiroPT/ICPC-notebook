/**
 * Author: jet notebook
 * Summary: Farthest pair on a convex hull
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Maximum squared distance by rotating
 *  calipers. Strictly convex CCW hull, no
 *  closing copy; n=0/1 gives 0, n=2 supported.
 *  \item $O(n)$ time, $O(1)$ extra space.
 *  Exact signs; follow Point's bounds.
 *  Reals may round near degeneracies.
 *  \end{itemize}
 * Usage: auto d2 = hullDiameter2(h);
 *  // h: CCW convex hull; d2: squared diameter
 */
#pragma once
#include "Predicates.h"

template<class T>
T hullDiameter2(
		const vec<Point<T>>& h) {
	int n = sz(h), j = 1;
	if (n < 2) return 0;
	if (n == 2) return (h[0]-h[1]).dist2();
	T ans = 0;
	rep(i,0,n) {
		int k = (i+1)%n;
		while (h[i].cross(h[k],h[(j+1)%n]) >
				h[i].cross(h[k],h[j])) j = (j+1)%n;
		ans = max(ans,(h[i]-h[j]).dist2());
		ans = max(ans,(h[i]-h[(j+1)%n]).dist2());
	}
	return ans;
}
