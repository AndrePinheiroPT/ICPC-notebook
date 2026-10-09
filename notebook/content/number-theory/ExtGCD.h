/**
 * Author: jet notebook, based on KACTL
 * Summary: Bezout coefficients for gcd and integer equations.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Find integer coefficients for ax+by=gcd(a,b).
 *  Use for modular inverses or integer equations (notes above).
 *  \item Pass signed a,b and variables x,y; returns g>=0 and
 *  writes x,y. Neither input may be its type's minimum.
 *  For a=b=0, g=0. x1,y1 solve the smaller recursive equation.
 *  \item $O(\log(\max(|a|,|b|)+1))$ time/recursion space.
 *  \end{itemize}
 * Usage: ll x,y; ll g = extGcd(30LL,18LL,x,y);
 *  // g=6 and 30*x+18*y=6
 */
#pragma once

template<class T>
T extGcd(T a, T b, T& x, T& y) {
	if (!b) {
		x = a < 0 ? -1 : 1; y = 0;
		return a < 0 ? -a : a;
	}
	T x1, y1;
	T g = extGcd(b,T(a%b),x1,y1);
	x = y1;
	y = x1 - (a/b)*y1;
	return g;
}
