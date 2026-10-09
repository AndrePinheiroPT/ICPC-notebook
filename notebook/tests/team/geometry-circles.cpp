#include "prelude.hpp"
using namespace std;
using ll = long long;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include "../../content/geometry/Point.h"
#pragma GCC diagnostic pop
#include "../../content/geometry/Circle2.h"
#include "../../content/geometry/EnclosingCircle.h"

static mt19937_64 gen(0xC18C1E2026ULL);
static uint64_t checks = 0, lines = 0, pairs = 0, circum = 0, enclosing = 0;
void check(bool ok, const char* message) {
	++checks;
	if (!ok) { cerr << "Circles FAILED: " << message << '\n'; abort(); }
}
int rnd(int n) { return int(gen()%unsigned(n)); }
using IP = pair<ll,ll>;
using I = __int128_t;

// Independent integer/rational oracle: solve the two bisector equations
// with determinants, then test all points using exact integer inequalities.
struct Exact {
	ll x, y, d;
	I rr;
};
Exact candidate(ll x, ll y, ll d, IP p) {
	if (d < 0) { x = -x; y = -y; d = -d; }
	I a = I(p.first)*d-x, b = I(p.second)*d-y;
	return {x,y,d,a*a+b*b};
}
optional<Exact> triple(IP a, IP b, IP c) {
	ll ux = b.first-a.first, uy = b.second-a.second;
	ll vx = c.first-a.first, vy = c.second-a.second;
	ll d = 2*(ux*vy-uy*vx);
	if (!d) return nullopt;
	ll bu = b.first*b.first+b.second*b.second-a.first*a.first-a.second*a.second;
	ll cv = c.first*c.first+c.second*c.second-a.first*a.first-a.second*a.second;
	return candidate(bu*vy-cv*uy,ux*cv-vx*bu,d,a);
}
Circle decimal(Exact q) {
	return {P2(ld(q.x)/ld(q.d),ld(q.y)/ld(q.d)),sqrt(ld(q.rr))/ld(q.d)};
}
Circle oracle(const vector<IP>& ps) {
	if (ps.empty()) return {P2(0,0),-1};
	optional<Exact> best;
	auto add = [&](Exact q) {
		for (auto [x,y] : ps) {
			I a = I(x)*q.d-q.x, b = I(y)*q.d-q.y;
			if (a*a+b*b > q.rr) return;
		}
		if (!best || q.rr*best->d*best->d < best->rr*q.d*q.d) best = q;
	};
	for (auto p : ps) add(candidate(p.first,p.second,1,p));
	for (int i = 0; i < sz(ps); ++i) for (int j = 0; j < i; ++j)
		add(candidate(ps[i].first+ps[j].first,ps[i].second+ps[j].second,2,ps[i]));
	for (int i = 0; i < sz(ps); ++i) for (int j = 0; j < i; ++j)
		for (int k = 0; k < j; ++k) {
			auto q = triple(ps[i],ps[j],ps[k]);
			if (q) add(*q);
		}
	check(bool(best), "exact candidate circle exists");
	return decimal(*best);
}

P2 transform(P2 p, ld scale, ld angle, P2 shift) {
	ld c = cos(angle), s = sin(angle);
	return shift+P2{p.x*c-p.y*s,p.x*s+p.y*c}*scale;
}
ld roundoff(P2 p) {
	return 128*numeric_limits<ld>::epsilon()*max({ld(1),abs(p.x),abs(p.y)});
}
void sameSet(const vector<P2>& got, const vector<P2>& want, ld tol) {
	check(sz(got) == sz(want), "intersection count");
	vector<bool> used(want.size());
	for (P2 p : got) {
		int found = -1;
		for (int j = 0; j < sz(want); ++j)
			if (!used[j] && sqrtl((p-want[j]).dist2()) <= tol) { found = j; break; }
		check(found >= 0, "intersection coordinates");
		used[found] = true;
	}
}

void testLine() {
	for (int test = 0; test < 2400; ++test) {
		ld r = rnd(21), h = rnd(51)-25;
		ld scale = test%3 == 0 ? 1e-6L : test%3 == 1 ? 1 : 1e6L;
		ld angle = ld(rnd(10000))/1000;
		P2 shift{ld(rnd(2001)-1000)*scale,ld(rnd(2001)-1000)*scale};
		if (test%11 == 0) shift = P2(1e12L,-1e12L), scale = 1;
		auto tr = [&](P2 p) { return transform(p,scale,angle,shift); };
		ld eps = 1e-11L*scale+roundoff(shift);
		Circle c{shift,r*scale};
		P2 a = tr(P2(-37,h)), b = tr(P2(29,h));
		vector<P2> want;
		if (abs(h) == r) want = {tr(P2(0,h))};
		else if (abs(h) < r) {
			ld x = sqrt(r*r-h*h);
			want = {tr(P2(-x,h)),tr(P2(x,h))};
		}
		auto got = circleLine(c,a,b,eps);
		sameSet(got,want,eps*20);
		sameSet(circleLine(c,b,a,eps),want,eps*20);
		for (P2 p : got) {
			check(abs(sqrtl((p-c.c).dist2())-c.r) <= eps*20, "line intersection on circle");
			check(abs((b-a).cross(p-a))/sqrtl((b-a).dist2()) <= eps*20,
			      "line intersection on supporting line");
		}
		++lines;
	}
	check(circleLine({P2(0,0),1},P2(-3,1+1e-13L),P2(2,1+1e-13L)).size() == 1,
	      "line near-tangent tolerance");
}

void testCirclePair() {
	for (int test = 0; test < 2600; ++test) {
		ld r = rnd(16), s = rnd(16), d = rnd(36);
		if (test%13 == 0) d = 0, s = r;
		ld scale = test%3 == 0 ? 1e-6L : test%3 == 1 ? 1 : 1e6L;
		ld angle = ld(rnd(10000))/1000;
		P2 shift{ld(rnd(201)-100)*scale,ld(rnd(201)-100)*scale};
		if (test%17 == 0) shift = P2(1e12L,-1e12L), scale = 1;
		auto tr = [&](P2 p) { return transform(p,scale,angle,shift); };
		Circle a{shift,r*scale}, b{tr(P2(d,0)),s*scale};
		ld eps = 1e-11L*scale+roundoff(shift);
		auto got = circleCircle(a,b,eps), rev = circleCircle(b,a,eps);
		bool infinite = d == 0 && r == s && r > 0;
		check(!got == infinite && !rev == infinite, "coincident circles");
		if (infinite) { ++pairs; continue; }
		vector<P2> want;
		if (r == 0 || s == 0) {
			if (d == r+s) want = {r == 0 ? a.c : b.c};
		} else if (d != 0 && d <= r+s && d >= abs(r-s)) {
			ld x = (r*r-s*s+d*d)/(2*d), y = sqrt(max(ld(0),r*r-x*x));
			if (d == r+s || d == abs(r-s)) want = {tr(P2(x,0))};
			else want = {tr(P2(x,y)),tr(P2(x,-y))};
		}
		sameSet(*got,want,eps*30); sameSet(*rev,want,eps*30);
		for (P2 p : *got) {
			check(abs(sqrtl((p-a.c).dist2())-a.r) <= eps*30, "intersection on first circle");
			check(abs(sqrtl((p-b.c).dist2())-b.r) <= eps*30, "intersection on second circle");
		}
		++pairs;
	}
	check(circleCircle({P2(0,0),0},{P2(0,0),0})->size() == 1,"zero radius coincidence");
	check(!circleCircle({P2(0,0),1},{P2(1e-14L,0),1}),"coincidence tolerance");
}

void testCircumcircle() {
	for (int test = 0; test < 2600; ++test) {
		vector<IP> ps(3);
		for (auto& [x,y] : ps) x = rnd(41)-20, y = rnd(41)-20;
		if (test%9 == 0) ps = {{-11,3},{2,3},{17,3}};
		auto want = triple(ps[0],ps[1],ps[2]);
		auto got = circumcircle(P2(ld(ps[0].first),ld(ps[0].second)),
		                        P2(ld(ps[1].first),ld(ps[1].second)),
		                        P2(ld(ps[2].first),ld(ps[2].second)));
		check(bool(got) == bool(want), "circumcircle collinearity");
		if (want) {
			Circle c = decimal(*want);
			ld tol = 1e-12L*max(ld(1),c.r);
			check(sqrtl((got->c-c.c).dist2()) <= tol && abs(got->r-c.r) <= tol,
			      "circumcircle exact rational oracle");
		}
		++circum;
	}
	for (ld scale : {1e-9L,ld(1),1e9L}) {
		P2 shift{1e12L*scale,-1e12L*scale};
		auto q = circumcircle(shift,shift+P2{2*scale,0},shift+P2{0,2*scale});
		check(bool(q), "translated circumcircle exists");
		check(sqrtl((q->c-(shift+P2{scale,scale})).dist2()) <= roundoff(shift)*8,
		      "translated circumcircle center");
		check(abs(q->r-sqrt(ld(2))*scale) <= roundoff(shift)*8,
		      "translated circumcircle radius");
		++circum;
	}
	auto q = circumcircle(P2(0,0),P2(1,0),P2(2,1e-12L));
	check(bool(q) && abs(q->c.x-0.5L) < 1e-10L && abs(q->c.y/1e12L-1) < 1e-12L,
	      "near-collinear nonzero determinant");
}

void mecCase(const vector<IP>& ps, ld scale = 1, P2 shift = P2(0,0), ld eps = -1) {
	Circle want = oracle(ps);
	vector<P2> p;
	for (auto [x,y] : ps) p.push_back(shift+P2{ld(x),ld(y)}*scale);
	if (eps < 0) eps = 1e-12L*scale+roundoff(shift);
	Circle got = enclosingCircle(p,eps);
	if (ps.empty()) {
		check(got.r == -1, "empty enclosing circle"); ++enclosing; return;
	}
	want.c = shift+want.c*scale; want.r *= scale;
	ld tol = eps*16+roundoff(shift)*16+1e-10L*scale;
	check(isfinite(got.r) && isfinite(got.c.x) && isfinite(got.c.y), "finite enclosing circle");
	if (abs(got.r-want.r) > tol || sqrtl((got.c-want.c).dist2()) > tol) {
		cerr << setprecision(20) << "MEC got " << got.c.x << ' ' << got.c.y << ' ' << got.r
		     << " want " << want.c.x << ' ' << want.c.y << ' ' << want.r << " tol " << tol << '\n';
		for (auto [x,y] : ps) cerr << x << ',' << y << ' ';
		cerr << '\n';
	}
	check(abs(got.r-want.r) <= tol, "MEC radius against exact oracle");
	check(sqrtl((got.c-want.c).dist2()) <= tol, "MEC center against exact oracle");
	for (P2 a : p) check(sqrtl((a-got.c).dist2()) <= got.r+roundoff(a), "MEC contains every point");
	++enclosing;
}

void testEnclosing() {
	mecCase({}); mecCase({{4,8}}); mecCase(vector<IP>(9,{4,8}));
	vector<vector<IP>> special{
		{{0,0},{1,0},{3,1}}, {{0,0},{2,0},{1,2}},
		{{-5,0},{5,0},{0,0},{2,0},{-3,0}},
		{{-2,-2},{-2,2},{2,-2},{2,2},{0,0}},
		{{-9,-18},{-3,-6},{1,2},{3,6},{7,14}},
		{{-1,0},{1,0},{0,0},{1,0},{-1,0}}
	};
	for (const auto& p : special) for (int repeat = 0; repeat < 20; ++repeat) mecCase(p);
	for (int test = 0; test < 3600; ++test) {
		vector<IP> ps(rnd(9));
		for (auto& [x,y] : ps) {
			x = rnd(41)-20; y = test%7 == 0 ? x*2+3 : rnd(41)-20;
		}
		mecCase(ps);
		if (test%20 == 0) {
			mecCase(ps,1e-9L);
			mecCase(ps,1e9L,P2(1e12L,-1e12L));
			mecCase(ps,1,P2(1e12L,-1e12L));
		}
	}
	// Absolute tolerance larger than radius, plus gradual point expansion.
	mecCase({{0,0},{1,0},{0,1}},1e-14L,P2(0,0),1e-12L);
	vector<IP> gradual;
	for (int i = 0; i < 80; ++i) gradual.emplace_back(i,0);
	mecCase(gradual,1e-13L,P2(0,0),1e-12L);
	for (int repeat = 0; repeat < 100; ++repeat) {
		vector<P2> p{P2(0,0),P2(1,0),P2(2,1e-12L),P2(0.5L,1e-15L),P2(1.5L,-1e-15L)};
		Circle c = enclosingCircle(p,1e-15L);
		check(abs(c.r-1) < 1e-11L && sqrtl((c.c-P2{1,5e-13L}).dist2()) < 1e-11L,
		      "near-collinear MEC uses diameter, not huge circumcircle");
		for (P2 a : p) check(sqrtl((a-c.c).dist2()) <= c.r+1e-17L,"near-collinear MEC containment");
		++enclosing;
	}
	// Thin rotated clouds whose extreme pair defines the exact MEC.
	// Repeated runs exercise distinct internal shuffles, including eps=0.
	for (int trial = 0; trial < 500; ++trial) {
		vector<P2> p{P2(-1,-2),P2(1,2)};
		for (int j = 0; j < 60; ++j) {
			ld x = ld(rnd(199)-99)/100;
			ld tiny = trial%2 ? 0 : ld(rnd(21)-10)*1e-15L;
			p.emplace_back(x,2*x+tiny);
		}
		Circle c = enclosingCircle(p,trial%3 ? 1e-15L : 0);
		check(sqrtl(c.c.dist2()) < 1e-11L && abs(c.r-sqrt(ld(5))) < 1e-11L,
		      "thin cloud/collinear extreme pair is MEC");
		for (P2 a : p) check(sqrtl((a-c.c).dist2()) <= c.r+1e-17L,"thin cloud containment");
		++enclosing;
	}
}

void roundoffRegression() {
	// This actual pre-fix witness once returned radius 19.3132 although
	// the circle centered at the origin with radius 1.04404 covers it.
	vector<P2> witness{
		P2(-1,-0.300000000000000000010842L),
		P2(1,0.300000000000000000010842L),
		P2(0.999999999999999997181074L,0.299999999999999999279006L),
		P2(-0.999999999999999999403689L,-0.299999999999999999929527L),
		P2(-0.999999999999999999403689L,-0.299999999999999999821107L),
		P2(0.999999999999999999620529L,0.299999999999999999902422L),
		P2(0.999999999999999999186848L,0.299999999999999999875317L),
		P2(-0.999999999999999998482117L,-0.299999999999999999441636L),
		P2(-0.999999999999999997994226L,-0.299999999999999999306111L),
		P2(-0.999999999999999997397915L,-0.299999999999999999333216L),
		P2(0.999999999999999999403689L,0.299999999999999999929527L),
		P2(-1,-0.300000000000000000010842L)
	};
	auto verify = [&](const vector<P2>& p, P2 center, ld tolerance) {
		ld bound = 0;
		for (P2 v : p) bound = max(bound,sqrtl((v-center).dist2()));
		Circle c = enclosingCircle(p,0);
		check(isfinite(c.r) && c.r <= bound+tolerance,
		      "roundoff regression respects constructive radius bound");
		for (P2 v : p)
			check(sqrtl((v-c.c).dist2()) <= c.r+numeric_limits<ld>::epsilon()*c.r,
			      "roundoff regression final radius contains points");
		++enclosing;
	};
	for (int repeat = 0; repeat < 300; ++repeat) verify(witness,P2(0,0),1e-12L);
	mt19937_64 rng(912867);
	for (int test = 0; test < 10000; ++test) {
		ld slope = ld(rng()%1001)/100;
		vector<P2> p{P2(-1,-slope),P2(1,slope)};
		for (int j = 0; j < 10; ++j) {
			ld x = (rng()%2 ? 1 : -1)*(1-ld(rng()%30)*1e-19L);
			p.emplace_back(x,slope*x+ld(int(rng()%3)-1)*1e-19L);
		}
		verify(p,P2(0,0),1e-12L);
		if (test%20 == 0) for (P2 shift : {P2{1e12L,-1e12L},P2{1e18L,-1e18L}}) {
			vector<P2> translated;
			for (P2 v : p) translated.push_back(v+shift);
			verify(translated,shift,roundoff(shift));
		}
	}
}

int main() {
	testLine(); testCirclePair(); testCircumcircle(); testEnclosing();
	roundoffRegression();
	cout << "Circles: " << lines << " circle-line, " << pairs << " circle-circle, "
	     << circum << " circumcircle, " << enclosing << " enclosing-circle cases; "
	     << checks << " checks passed (exact rational MEC oracle)\n";
}
