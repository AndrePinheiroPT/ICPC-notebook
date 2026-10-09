/**
 * Author: jet notebook
 * Summary: Twice the signed polygon area
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Twice signed area; positive for CCW.
 *  Empty/point/segment gives 0; divide by 2.
 *  \item $O(n)$ time, $O(1)$ extra space.
 *  Simple polygon; products and sum must fit T.
 *  \end{itemize}
 * Usage: auto twice = polyArea2(p);
 *  // p: polygon vertices in boundary order
 */
#pragma once
#include "Predicates.h"

template<class T>
T polyArea2(
		const vec<Point<T>>& p) {
	T ans = 0;
	rep(i,0,sz(p))
		ans += p[i].cross(p[(i+1)%sz(p)]);
	return ans;
}
