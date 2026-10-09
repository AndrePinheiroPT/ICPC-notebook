/**
 * Author: jet notebook
 * Summary: Minimum enclosing circle
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Randomized minimum enclosing circle.
 *  Empty input returns radius -1. Duplicates and
 *  collinear points are allowed.
 *  \item Boundary tolerance is absolute eps
 *  ($\ge0$) plus machine roundoff by scale;
 *  minimality is approximate within that scale.
 *  Scale/conditioning must suit long double.
 *  Final radius covers every point.
 *  \item Expected $O(n)$, worst $O(n^3)$ time;
 *  $O(n)$ space, including the input copy.
 *  \end{itemize}
 * Usage: Circle c = enclosingCircle(p);
 *  // p: points; c.c: center; c.r: radius
 */
#pragma once
#include "Circle2.h"

Circle enclosingCircle(vec<P2> p,
		ld eps = 1e-12L) {
	static mt19937 rng(random_device{}());
	shuffle(p.begin(),p.end(),rng);
	Circle c{P2(0,0),-1};
	auto out = [&](P2 a) {
		ld s = max({abs(a.x),abs(a.y),
			abs(c.c.x),abs(c.c.y),c.r});
		ld e = eps+
			16*numeric_limits<ld>::epsilon()*s;
		ld d = sqrtl((a-c.c).dist2());
		return c.r < 0 || d > c.r+e;
	};
	auto diameter = [](P2 a, P2 b) {
		return Circle{a+(b-a)/2,
			sqrtl((b-a).dist2())/2};
	};
	rep(i,0,sz(p)) if (out(p[i])) {
		c = {p[i],0};
		rep(j,0,i) if (out(p[j])) {
			c = diameter(p[i],p[j]);
			rep(k,0,j)
				if (out(p[k])) {
					auto q = circumcircle(p[i],p[j],p[k]);
					if (q) c = *q;
					else {
						c = diameter(p[i],p[j]);
						for (int v : {i,j}) {
							auto d = diameter(p[v],p[k]);
							if (d.r > c.r) c = d;
						}
					}
				}
		}
	}
	for (P2 a : p)
		c.r = max(c.r,sqrtl((a-c.c).dist2()));
	return c;
}
