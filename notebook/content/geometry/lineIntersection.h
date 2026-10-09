/**
 * Author: Victor Lecomte, chilli
 * Date: 2019-05-05
 * License: CC0
 * Source: Original KACTL lineIntersection.h
 * Summary: Intersection of two infinite lines
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Lines through distinct endpoint pairs.
 *  Returns \{1,point\}, \{0,(0,0)\} if parallel,
 *  or \{-1,(0,0)\} if coincident. $O(1)$.
 *  \item Real points for fractional intersections;
 *  integer division truncates.
 *  Products of three coordinates must fit the type.
 *  Large coordinates can cancel in the numerator;
 *  nearly parallel real lines amplify rounding errors.
 *  \end{itemize}
 * Usage: auto [status,p] = lineInter(a,b,c,d);
 *  // a,b and c,d define the lines; use P2 for fractional points
 */
#pragma once

#include "Point.h"

template<class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
	auto d = (e1 - s1).cross(e2 - s2);
	if (d == 0) // if parallel
		return {-(s1.cross(e1, s2) == 0), P(0, 0)};
	auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
	return {1, (s1 * p + e1 * q) / d};
}
