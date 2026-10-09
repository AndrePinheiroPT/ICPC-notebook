#include "prelude.hpp"
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using ll = long long;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include "../../content/geometry/Point.h"
#pragma GCC diagnostic pop
#include "../../content/geometry/PolyArea.h"
#include "../../content/geometry/PointInPoly.h"
#include "../../content/geometry/Hull.h"
#include "../../content/geometry/PointInConvex.h"
#include "../../content/geometry/HullDiameter2.h"
#include "../../content/geometry/ConvexCut.h"
using boost::multiprecision::cpp_int;
using boost::multiprecision::cpp_rational;
using IP = Point<ll>;
using Q = cpp_rational;
static mt19937_64 rng(0xa281cb0131ULL);
static uint64_t checks = 0, hulls = 0, queries = 0;
static uint64_t polys = 0, cuts = 0, pairDistances = 0;
void require(bool ok, const char* msg) {
	++checks;
	if (!ok) { cerr << "FAILED Polygons: " << msg << '\n'; abort(); }
}
int pick(int n) { return int(rng()%uint64_t(n)); }
cpp_int cross(IP a, IP b, IP c) {
	return (cpp_int(b.x)-a.x)*(cpp_int(c.y)-a.y)
	     - (cpp_int(b.y)-a.y)*(cpp_int(c.x)-a.x);
}
cpp_int d2(IP a, IP b) {
	cpp_int x = cpp_int(a.x)-b.x, y = cpp_int(a.y)-b.y;
	return x*x+y*y;
}
bool on(IP a, IP b, IP q) {
	return cross(a,b,q) == 0 && min(a.x,b.x) <= q.x && q.x <= max(a.x,b.x)
	     && min(a.y,b.y) <= q.y && q.y <= max(a.y,b.y);
}
int winding(const vector<IP>& p, IP q) {
	int w = 0;
	for (int i = 0; i < sz(p); ++i) {
		IP a = p[i], b = p[(i+1)%sz(p)];
		if (on(a,b,q)) return 0;
		if (a.y <= q.y && b.y > q.y && cross(a,b,q) > 0) ++w;
		if (b.y <= q.y && a.y > q.y && cross(a,b,q) < 0) --w;
	}
	return w ? 1 : -1;
}
cpp_int area(const vector<IP>& p) {
	cpp_int s = 0;
	for (int i = 0; i < sz(p); ++i) {
		IP a = p[i], b = p[(i+1)%sz(p)];
		s += cpp_int(a.x)*b.y-cpp_int(a.y)*b.x;
	}
	return s;
}
// Gift wrapping uses exact arbitrary-precision orientation, independently
// of the production sorting/monotone-chain implementation.
vector<IP> gift(vector<IP> p) {
	sort(p.begin(),p.end());
	p.erase(unique(p.begin(),p.end()),p.end());
	if (sz(p) < 2) return p;
	vector<IP> h;
	int u = 0;
	do {
		h.push_back(p[u]);
		int v = (u+1)%sz(p);
		for (int w = 0; w < sz(p); ++w) if (w != u) {
			cpp_int c = cross(p[u],p[v],p[w]);
			if (c < 0 || (c == 0 && d2(p[u],p[w]) > d2(p[u],p[v]))) v = w;
		}
		u = v;
		require(sz(h) <= sz(p), "gift oracle terminates");
	} while (u);
	return h;
}
template<class T> vector<Point<T>> convert(const vector<IP>& p) {
	vector<Point<T>> out;
	for (IP v : p) out.emplace_back(T(v.x),T(v.y));
	return out;
}
template<class T> bool same(T got, const cpp_int& want) {
	if constexpr (is_floating_point_v<T>) return got == want.convert_to<T>();
	else return cpp_int(got) == want;
}
template<class T> void polygonCase(const vector<IP>& p, const vector<IP>& qs) {
	auto a = convert<T>(p);
	require(same(polyArea2(a),area(p)), "signed shoelace exact area");
	for (IP q : qs) {
		require(pointInPoly(a,Point<T>(T(q.x),T(q.y))) == winding(p,q), "parity vs winding oracle");
		++queries;
	}
	reverse(a.begin(),a.end());
	require(same(polyArea2(a),-area(p)), "reversed area sign");
	for (IP q : qs) {
		require(pointInPoly(a,Point<T>(T(q.x),T(q.y))) == winding(p,q), "clockwise point query");
		++queries;
	}
	++polys;
}
template<class T> void hullCase(const vector<IP>& p, vector<IP> qs) {
	auto h = hull(convert<T>(p));
	auto want = gift(p);
	require(h == convert<T>(want), "monotone chain vs exact gift wrapping");
	cpp_int best = 0;
	for (IP a : p) for (IP b : p) {
		best = max(best,d2(a,b));
		++pairDistances;
	}
	require(same(hullDiameter2(h),best), "calipers vs all input pairs");
	for (IP v : want) qs.push_back(v);
	for (IP q : qs) {
		require(pointInConvex(h,Point<T>(T(q.x),T(q.y))) == winding(want,q), "convex binary query");
		++queries;
	}
	if (!h.empty()) {
		int k = pick(sz(h));
		rotate(h.begin(),h.begin()+k,h.end());
		for (IP q : qs) {
			require(pointInConvex(h,Point<T>(T(q.x),T(q.y))) == winding(want,q), "rotated convex query");
			++queries;
		}
		require(same(hullDiameter2(h),best), "rotated calipers");
	}
	polygonCase<T>(want,qs);
	++hulls;
}

struct RP { Q x, y; };
bool req(const RP& a, const RP& b) { return a.x == b.x && a.y == b.y; }
Q rcross(const RP& a, const RP& b, const RP& c) {
	return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
Q rd2(const RP& a, const RP& b) {
	Q x = a.x-b.x, y = a.y-b.y;
	return x*x+y*y;
}
vector<RP> rgift(vector<RP> p) {
	sort(p.begin(),p.end(),[](const RP& a,const RP& b) {
		return a.x != b.x ? a.x < b.x : a.y < b.y;
	});
	p.erase(unique(p.begin(),p.end(),req),p.end());
	if (sz(p) < 2) return p;
	vector<RP> out;
	int u = 0;
	do {
		out.push_back(p[u]);
		int v = (u+1)%sz(p);
		for (int w = 0; w < sz(p); ++w) if (w != u) {
			Q c = rcross(p[u],p[v],p[w]);
			if (c < 0 || (c == 0 && rd2(p[u],p[w]) > rd2(p[u],p[v]))) v = w;
		}
		u = v;
		require(sz(out) <= sz(p), "rational gift oracle terminates");
	} while (u);
	return out;
}
// Intersect boundary supporting lines with rational Cramer's rule;
// then gift-wrap the candidate set. No interpolation or clipping loop.
vector<RP> rcut(const vector<RP>& p, RP a, RP b) {
	vector<RP> candidates;
	for (const RP& v : p) if (rcross(a,b,v) >= 0) candidates.push_back(v);
	Q x = b.y-a.y, y = a.x-b.x, z = x*a.x+y*a.y;
	for (int i = 0; i < sz(p); ++i) {
		RP u = p[i], v = p[(i+1)%sz(p)];
		if (rcross(a,b,u)*rcross(a,b,v) >= 0) continue;
		Q e = v.y-u.y, f = u.x-v.x, g = e*u.x+f*u.y;
		Q det = x*f-e*y;
		candidates.push_back({(z*f-g*y)/det,(x*g-e*z)/det});
	}
	return rgift(candidates);
}
ld real(const Q& q) { return q.convert_to<ld>(); }
bool near(ld a, ld b) { return abs(a-b) <= 2e-12L*max({1.L,abs(a),abs(b)}); }
void verifyCut(const vector<P2>& got, const vector<RP>& want, P2 a, P2 b) {
	require(sz(got) == sz(want), "clip exact rational vertex count");
	vector<bool> seen(want.size());
	for (P2 v : got) {
		bool found = false;
		for (int j = 0; j < sz(want); ++j)
			if (!seen[j] && near(v.x,real(want[j].x)) && near(v.y,real(want[j].y))) {
				seen[j] = true; found = true; break;
			}
		require(found, "clip vertex vs rational Cramer/gift oracle");
		ld scale = max(1.L,sqrtl((b-a).dist2())*max(1.L,sqrtl((v-a).dist2())));
		require(a.cross(b,v) >= -2e-12L*scale, "clip inside requested halfplane");
	}
	for (int i = 0; i < sz(got); ++i) if (sz(got) > 1)
		require(!(got[i] == got[(i+1)%sz(got)]), "clip consecutive duplicates removed");
	Q expected = 0;
	for (int i = 0; i < sz(want); ++i) {
		RP u = want[i], v = want[(i+1)%sz(want)];
		expected += u.x*v.y-u.y*v.x;
	}
	require(near(abs(polyArea2(got)),real(expected)), "clip area exact rational oracle");
	++cuts;
}
void cutCase(vector<IP> p, IP a, IP b) {
	vector<RP> rp;
	for (IP v : p) rp.push_back({v.x,v.y});
	auto want = rcut(rp,{a.x,a.y},{b.x,b.y});
	verifyCut(convexCut(convert<ld>(p),toReal(a),toReal(b)),want,toReal(a),toReal(b));
	reverse(p.begin(),p.end());
	verifyCut(convexCut(convert<ld>(p),toReal(a),toReal(b)),want,toReal(a),toReal(b));
}

void exactTests() {
	vector<IP> grid;
	for (int x = -4; x <= 4; ++x) for (int y = -4; y <= 4; ++y) grid.emplace_back(x,y);
	vector<vector<IP>> fixed = {{},{IP(0,0)},{IP(0,0),IP(0,0)},{IP(-2,-2),IP(2,2)},
		{IP(0,0),IP(2,0),IP(4,0),IP(2,0)},{IP(0,0),IP(0,4),IP(4,4),IP(4,0),IP(2,2)},
		{IP(-4,-4),IP(4,-4),IP(4,4),IP(-4,4),IP(0,0),IP(4,0)},
		{IP(0,0),IP(4,0),IP(0,4)},{IP(0,0),IP(2,0),IP(3,2),IP(1,4),IP(-2,2)}};
	for (const auto& p : fixed) {
		hullCase<ll>(p,grid); hullCase<ll>(p,grid); hullCase<ld>(p,grid);
	}
	vector<IP> concave = {IP(0,0),IP(4,0),IP(4,4),IP(3,4),IP(3,1),IP(1,1),IP(1,4),IP(0,4)};
	polygonCase<ll>(concave,grid); polygonCase<ld>(concave,grid);
	for (int test = 0; test < 1600; ++test) {
		vector<IP> p, qs;
		int n = pick(36);
		for (int i = 0; i < n; ++i) p.emplace_back(2*(pick(31)-15),2*(pick(31)-15));
		for (int i = 0; i < 18; ++i) qs.emplace_back(pick(81)-40,pick(81)-40);
		auto h = gift(p);
		for (int i = 0; i < sz(h); ++i)
			qs.emplace_back((h[i].x+h[(i+1)%sz(h)].x)/2,(h[i].y+h[(i+1)%sz(h)].y)/2);
		if (test%3 == 0) hullCase<ll>(p,qs);
		if (test%3 == 1) hullCase<ll>(p,qs);
		if (test%3 == 2) hullCase<ld>(p,qs);
		if (test < 300) {
			IP a{pick(41)-20,pick(41)-20}, b{pick(41)-20,pick(41)-20};
			if (a == b) ++b.x;
			cutCase(h,a,b);
			if (sz(h) >= 2) cutCase(h,h[0],h[1]);
		}
	}
	for (int test = 0; test < 250; ++test) {
		int n = 2+pick(15);
		vector<IP> p, qs;
		for (int x = 0; x < n; ++x) p.emplace_back(2*x,2+2*pick(7));
		for (int x = n-1; x >= 0; --x) p.emplace_back(2*x,-2-2*pick(7));
		for (int i = 0; i < 40; ++i) qs.emplace_back(pick(2*n+4)-2,pick(35)-17);
		for (IP v : p) qs.push_back(v);
		polygonCase<ll>(p,qs);
	}
	const ll big = 100000000LL;
	for (int test = 0; test < 100; ++test) {
		vector<IP> p, qs = {IP(0,0),IP(big,big),IP(-big,-big),IP(big,0),IP(0,-big)};
		for (int i = 0; i < 20; ++i) {
			ll x = ll(rng()%200000001ULL)-big;
			ll y = ll(rng()%200000001ULL)-big;
			p.emplace_back(x,y);
		}
		hullCase<ll>(p,qs);
	}
	hullCase<ll>({IP(-big,-big),IP(big,-big),IP(big,big),IP(-big,big)},
		{IP(0,0),IP(big,0),IP(0,-big),IP(big,big),IP(big+1,0)});
	// Near-collinear integer data with a large translation.
	hullCase<ll>({IP(big,big),IP(big-1,big-2),IP(big-2,big-3),IP(big-3,big-4)},
		{IP(big-1,big-1),IP(big-2,big-3),IP(big-3,big-3)});
	vector<P2> dyadic = {P2(0.125L,-0.25L),P2(3.625L,-0.25L),
		P2(3.625L,1.75L),P2(0.125L,1.75L)};
	auto points = dyadic;
	points.push_back(P2(2,-0.25L)); points.push_back(dyadic[0]);
	points.push_back(P2(1,1));
	auto dh = hull(points);
	require(dh == dyadic && polyArea2(dh) == 14.L, "dyadic hull/area");
	require(hullDiameter2(dh) == 16.25L, "dyadic squared diameter");
	for (auto [q,want] : vector<pair<P2,int>>{{P2(0.125L,0),0},{P2(1,1),1},
		{P2(0,0),-1},{P2(3.625L,1.75L),0},{P2(3.75L,1.75L),-1}}) {
		require(pointInPoly(dh,q) == want && pointInConvex(dh,q) == want,
			"dyadic polygon queries");
		queries += 2;
	}
	++hulls; ++polys;
}
void cutTests() {
	vector<vector<IP>> ps = {{},{IP(0,0)},{IP(-2,0),IP(2,0)},
		{IP(-2,-2),IP(2,-2),IP(2,2),IP(-2,2)},{IP(0,0),IP(4,0),IP(0,4)}};
	vector<pair<IP,IP>> lines = {{IP(0,0),IP(1,0)},{IP(1,0),IP(0,0)},{IP(0,0),IP(0,1)},
		{IP(2,-4),IP(2,4)},{IP(2,4),IP(2,-4)},{IP(-2,-4),IP(-2,4)},{IP(3,0),IP(3,1)},
		{IP(2,2),IP(3,1)},{IP(2,2),IP(1,3)},{IP(0,4),IP(4,0)},{IP(4,0),IP(0,4)}};
	for (const auto& p : ps) for (auto [a,b] : lines) cutCase(p,a,b);
	// Segment endpoint intersection is computed once, avoiding two rounded
	// copies of the same intersection from traversing the segment both ways.
	cutCase({IP(-11,7),IP(19,-13)},IP(0,0),IP(1,1));
	vector<P2> p = {P2(-20,-20),P2(20,-20),P2(20,20),P2(-20,20)};
	vector<RP> rp = {{-20,-20},{20,-20},{20,20},{-20,20}};
	for (auto [a,b] : vector<pair<IP,IP>>{{IP(-8,-9),IP(10,-8)},{IP(14,-12),IP(8,15)},
		{IP(8,14),IP(-13,10)},{IP(-14,11),IP(-9,-13)},{IP(-5,-7),IP(9,5)},{IP(8,10),IP(-4,-6)}}) {
		p = convexCut(p,toReal(a),toReal(b)); rp = rcut(rp,{a.x,a.y},{b.x,b.y});
		verifyCut(p,rp,toReal(a),toReal(b));
	}
	// Opposite strict signs: interpolation stays on the edge even for a
	// tiny distance from the cutting line. No tolerance-induced extrapolation.
	ld e = 1e-20L;
	auto tiny = convexCut({P2(-1,-e),P2(1,e)},P2(0,0),P2(1,0));
	require(sz(tiny) == 2 && tiny[0] == P2(0,0) && tiny[1] == P2(1,e),
		"near-degenerate segment interpolation stays bounded");
	++cuts;
}
int main() {
	exactTests(); cutTests();
	cout << "Polygons: " << hulls << " hulls, " << polys << " polygons, "
	     << queries << " location queries, " << pairDistances << " pair distances, "
	     << cuts << " cuts; " << checks << " checks passed.\n";
}
