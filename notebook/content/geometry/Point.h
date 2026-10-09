/**
 * Author: Ulf Lundstrom
 * Date: 2009-02-26
 * License: CC0
 * Source: Original KACTL Point.h
 * Summary: 2D points and vectors
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Point<T>(x,y) stores a point or vector; use ll for integer
 *  predicates and long double for real constructions. Arithmetic must fit T.
 *  Addition/subtraction are coordinate-wise; multiplication/division use a scalar.
 *  \item dot(p) is the scalar product; cross(p) the signed determinant.
 *  a.cross(b,c) is twice the oriented triangle area, positive counterclockwise.
 *  dist2() is squared length; (a-b).dist() is Euclidean distance.
 *  \item dist/angle return double. unit/normal require nonzero real vectors;
 *  perp() rotates 90 degrees; rotate(angle) uses radians. Sorting compares
 *  (x,y) exactly, without epsilon. All operations are $O(1)$.
 *  \end{itemize}
 * Usage: Point<ll> a(1,2), b(4,6);
 *  ll d2 = (b-a).dist2(); // 25
 */
#pragma once

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};
