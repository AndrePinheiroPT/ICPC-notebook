/**
 * Author: jet notebook
 * Summary: Circle intersections and circumcircle
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Circle--line/circle intersections, $O(1)$.
 *  0/1/2 points; nullopt treats the circles as
 *  coincident within eps (infinite case).
 *  Radius zero works.
 *  \item Finite data, radius $\ge0$, distinct line
 *  endpoints; eps $\ge0$ is absolute distance
 *  tolerance. Scale must suit long double.
 *  \item Circumcircle returns nullopt if collinear;
 *  near-collinearity is ill-conditioned. $O(1)$.
 *  \end{itemize}
 * Usage: Circle c{center,radius};
 *  auto p = circleLine(c,a,b); // line a->b
 *  auto q = circleCircle(c,d); // d: other circle
 *  auto e = circumcircle(a,b,p0); // three points
 */
#pragma once
#include "Predicates.h"

struct Circle { P2 c; ld r; };

vec<P2> circleLine(Circle c, P2 a, P2 b,
		ld eps = 1e-12L) {
	P2 u = (b-a)/sqrtl((b-a).dist2());
	P2 v = u.perp();
	ld h = (a-c.c).dot(v), d = abs(h);
	if (d > c.r+eps) return {};
	P2 p = c.c+v*h;
	if (d >= c.r-eps) return {p};
	P2 q = u*sqrt(max(ld(0),(c.r-d)*(c.r+d)));
	return {p-q,p+q};
}

optional<vec<P2>> circleCircle(Circle a,
		Circle b, ld eps = 1e-12L) {
	P2 v = b.c-a.c;
	ld d = sqrtl(v.dist2()), sum = a.r+b.r;
	ld diff = abs(a.r-b.r);
	if (a.r == 0 || b.r == 0) {
		if (abs(d-sum) > eps) return vec<P2>{};
		return vec<P2>{a.r == 0 ? a.c : b.c};
	}
	if (d <= eps) {
		if (diff <= eps) return nullopt;
		return vec<P2>{};
	}
	if (d > sum+eps || d < diff-eps)
		return vec<P2>{};
	ld x = (d+(a.r-b.r)/d*sum)/2;
	P2 u = v/d, p = a.c+u*x;
	if (d >= sum-eps || d <= diff+eps)
		return vec<P2>{p};
	ld h = sqrt(max(ld(0),(a.r-x)*(a.r+x)));
	P2 q = u.perp()*h;
	return vec<P2>{p-q,p+q};
}

optional<Circle> circumcircle(P2 a, P2 b, P2 c) {
	P2 u = b-a, v = c-a;
	ld s = max({abs(u.x),abs(u.y),
		abs(v.x),abs(v.y)});
	if (s == 0) return nullopt;
	ld d = (u.cross(v)/s)/s*2;
	if (d == 0) return nullopt;
	u = u/s; v = v/s;
	P2 w = (u.perp()*v.dist2()-
		v.perp()*u.dist2())/d*s;
	return Circle{a+w,sqrtl(w.dist2())};
}
