#include "prelude.hpp"
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using ll = long long;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include "../../content/geometry/Point.h"
#pragma GCC diagnostic pop
#include "../../content/geometry/LineSegInter.h"
#include "../../content/geometry/Projection2.h"
using boost::multiprecision::cpp_int;
using I = Point<ll>;
using Z = cpp_int;
static long long checks=0, segmentCases=0, lineCases=0;
static void checkAt(bool ok,int line) {
	++checks;
	if (!ok) {cerr<<"Failure line "<<line<<" check "<<checks<<'\n';abort();}
}
#define ck(...) checkAt((__VA_ARGS__),__LINE__)
static bool near(ld a,ld b,ld tol=2e-16L) {
	return abs(a-b) <= tol*max({ld(1),abs(a),abs(b)});
}
static bool nearP(P2 a,P2 b) {
	return near(a.x,b.x) && near(a.y,b.y);
}
static Z cross(I a,I b,I c) {
	return (Z(b.x)-a.x)*(Z(c.y)-a.y)
		-(Z(b.y)-a.y)*(Z(c.x)-a.x);
}
static bool on(I a,I b,I p) {
	return cross(a,b,p)==0 && min(a.x,b.x)<=p.x && p.x<=max(a.x,b.x)
		&& min(a.y,b.y)<=p.y && p.y<=max(a.y,b.y);
}
struct Rat {
	Z num,den;
	Rat(Z n,Z d=1):num(move(n)),den(move(d)) {
		if (den<0) num=-num,den=-den;
		assert(den>0);
	}
	ld value() const {return num.convert_to<ld>()/den.convert_to<ld>();}
	bool between(ll a,ll b) const {
		return Z(min(a,b))*den<=num && num<=Z(max(a,b))*den;
	}
};
struct Sol {int count; Rat x,y;};
static Sol cramer(I a,I b,I c,I d) {
	Z A=Z(a.y)-b.y,B=Z(b.x)-a.x,C=Z(a.x)*b.y-Z(a.y)*b.x;
	Z D=Z(c.y)-d.y,E=Z(d.x)-c.x,F=Z(c.x)*d.y-Z(c.y)*d.x;
	Z det=A*E-D*B;
	if (det==0) return {cross(a,b,c)==0?-1:0,{0},{0}};
	return {1,{B*F-E*C,det},{C*D-F*A,det}};
}
static void testSegments(I a,I b,I c,I d,bool rawLine=true) {
	++segmentCases;
	vector<P2> want;
	if (!(a==b) && !(c==d)) {
		auto s=cramer(a,b,c,d);
		if (s.count==1) {
			if (s.x.between(a.x,b.x) && s.x.between(c.x,d.x)
				&& s.y.between(a.y,b.y) && s.y.between(c.y,d.y))
				want.emplace_back(s.x.value(),s.y.value());
		} else if (s.count==-1) {
			vector<I> q;
			for (auto p : {a,b,c,d}) if (on(a,b,p)&&on(c,d,p)) q.push_back(p);
			sort(q.begin(),q.end());q.erase(unique(q.begin(),q.end()),q.end());
			for (auto p:q) want.push_back(toReal(p));
		}
	} else if (a==b) {
		if (on(c,d,a)) want.push_back(toReal(a));
	} else if (on(a,b,c)) want.push_back(toReal(c));
	auto got=segInter(a,b,c,d);
	ck(got.size()==want.size());
	for (size_t i=0;i<got.size();++i) ck(nearP(got[i],want[i]));
	auto rev=segInter(b,a,d,c);
	ck(rev.size()==got.size());
	for (size_t i=0;i<got.size();++i) ck(nearP(rev[i],got[i]));
	if (rawLine && !(a==b) && !(c==d)) {
		++lineCases;
		auto s=cramer(a,b,c,d);auto z=lineInter(toReal(a),toReal(b),toReal(c),toReal(d));
		ck(z.first==s.count);
		if (z.first==1) ck(nearP(z.second,P2(s.x.value(),s.y.value())));
	}
}
static void wideSegmentRegression(mt19937_64& rng) {
	const ll span=1000000000000000000LL;
	vector<I> shifts{I(),I(0,span),I(span,0),I(-span,span),
		I(span,-span),I(1000000000000LL,-1000000000000LL),I(-17,23)};
	auto allOrders=[&](I a,I b,I c,I d) {
		for (int order=0;order<8;++order) {
			I A=a,B=b,C=c,D=d;
			if (order&1) swap(A,B);
			if (order&2) swap(C,D);
			if (order&4) swap(A,C),swap(B,D);
			// Raw Cramer is unchanged and can cancel here; the segment
			// wrapper conditions the construction, tested against exact rationals.
			testSegments(A,B,C,D,false);
		}
	};
	for (I t:shifts) {
		allOrders(I(-span,0)+t,I(span,0)+t,I(1,-1)+t,I(1,1)+t);
		allOrders(I(0,-span)+t,I(0,span)+t,I(-1,1)+t,I(1,1)+t);
	}
	for (int it=0;it<1000;++it) {
		ll x=ll(rng()%2001)-1000, dx=ll(rng()%11)-5;
		ll low=1+ll(rng()%2), high=1+ll(rng()%2);
		I t=shifts[size_t(it)%shifts.size()];
		allOrders(I(-span,0)+t,I(span,0)+t,I(x,-low)+t,I(x+dx,high)+t);
	}
}
int main() {
	Point<double> v(3,4), axis(1,0);
	ck(near(v.dist(),5));
	ck(near(v.unit().dist(),1,1e-15L));
	ck(near(v.normal().dot(v),0,1e-15L));
	auto rotated=axis.rotate(acos(-1.0)/2);
	ck(near(rotated.x,0,1e-15L) && near(rotated.y,1,1e-15L));
	ck(near(Point<double>(0,1).angle(),acos(-1.0)/2,1e-15L));
	mt19937_64 rng(804271);
	for (ll lim : {1000LL,1000000000LL,100000000LL}) {
		uniform_int_distribution<ll> pick(-lim,lim);
		for (int it=0;it<10000;++it) {
			I a(pick(rng),pick(rng)),b(pick(rng),pick(rng)),p(pick(rng),pick(rng));
			Z dot=Z(a.x)*b.x+Z(a.y)*b.y;
			Z det=Z(a.x)*b.y-Z(a.y)*b.x;
			ck(Z(a.dot(b))==dot);ck(Z(a.cross(b))==det);
			Z cr=cross(a,b,p);
			ck(Z(a.cross(b,p))==cr);
			ck(side(a,b,p)==((cr>0)-(cr<0)));
			ck(onSeg(a,b,p)==on(a,b,p));
			ck(near(sqrtl(a.dist2()),hypotl(ld(a.x),ld(a.y))));
			ck(a.perp().dot(a)==0 && a.perp().dist2()==a.dist2());
		}
	}
	vector<I> grid;
	for (ll x=-1;x<=1;++x) for (ll y=-1;y<=1;++y) grid.emplace_back(x,y);
	for (auto a:grid) for (auto b:grid) for (auto c:grid) for (auto d:grid)
		testSegments(a,b,c,d);
	for (int it=0;it<5000;++it) {
		ll scale=it%2 ? 1000LL : 1;
		auto point=[&] {return I((ll(rng()%1001)-500)*scale,(ll(rng()%1001)-500)*scale);};
		auto a=point(),b=point(),c=point(),d=point();
		testSegments(a,b,c,d);
	}
	I lo(-100000000LL,-100000000LL);
	I hi(100000000LL,100000000LL);
	testSegments(lo,hi,I(-hi.x,hi.y),I(hi.x,-hi.y));
	testSegments(lo,hi,lo,lo);
	Point<ll> ia(-1000000000,-1000000000),ib(1000000000,1000000000);
	ck(ia.cross(ib,Point<ll>(-1000000000,1000000000))==4000000000000000000LL);
	ck(segInter(ia,ib,Point<ll>(-1000000000,1000000000),Point<ll>(1000000000,-1000000000))==vector<P2>{P2(0,0)});
	wideSegmentRegression(rng);
	for (int it=0;it<10000;++it) {
		auto point=[&] {return P2(ld(ll(rng()%2001)-1000),ld(ll(rng()%2001)-1000));};
		auto a=point(),b=point(),p=point();
		if (a==b) b.x+=1;
			P2 u=b-a,q=project(a,b,p),r=reflect(a,b,p);
			ld sc=max({ld(1),sqrtl(a.dist2()),sqrtl(b.dist2()),sqrtl(p.dist2())});
		ld A=a.y-b.y,B=b.x-a.x,C=a.x*b.y-a.y*b.x;
		ld val=A*p.x+B*p.y+C;
		P2 expected{p.x-A*val/(A*A+B*B),p.y-B*val/(A*A+B*B)};
			ck(nearP(q/sc,expected/sc));
			ck(nearP((p+r)/2/sc,q/sc));
			ck(nearP(reflect(a,b,r)/sc,p/sc));
		ld dist=abs(val)/hypotl(A,B);
			ck(near(lineDist(a,b,p)/sc,dist/sc));
		ld t=(q-a).dot(u)/u.dist2();
		ld best=min(sqrtl((p-a).dist2()),sqrtl((p-b).dist2()));
		if (t>=0 && t<=1) best=min(best,dist);
			ck(near(segDist(a,b,p)/sc,best/sc));
			ck(near(segDist(a,a,p)/sc,sqrtl((p-a).dist2())/sc));
	}
	ck(onSeg(P2(0,0),P2(10,0),P2(5,1e-9L),ld(1e-7L)));
	ck(!onSeg(P2(0,0),P2(10,0),P2(5,1e-9L)));
	ck(geoSgn(ld(1e-15L),ld(1e-12L))==0);
	cout<<"Primitives: "<<checks<<" checks, "<<segmentCases
		<<" segment cases, "<<lineCases<<" line cases; cpp_int oracles passed\n";
}
