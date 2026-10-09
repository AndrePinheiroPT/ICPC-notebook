#include "prelude.hpp"
using namespace std;
using ll = long long;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include "../../content/geometry/Point.h"
#pragma GCC diagnostic pop
#include "../../content/geometry/LineSegInter.h"
#include "../../content/geometry/Projection2.h"
#include "../../content/geometry/PolyArea.h"
#include "../../content/geometry/PointInPoly.h"
#include "../../content/geometry/Hull.h"
#include "../../content/geometry/PointInConvex.h"
#include "../../content/geometry/HullDiameter2.h"
#include "../../content/geometry/ConvexCut.h"
#include "../../content/geometry/EnclosingCircle.h"
#include "../../content/geometry/NearestPair.h"
#include "../../content/geometry/RectUnion.h"
#include "../../content/math/IntDivision.h"
using IP = Point<ll>;
using W = __int128_t;
static mt19937_64 rng(0x26AD0E582CULL);
static uint64_t checks=0, polygonCases=0, gridCases=0, analyticCases=0;
static void checkAt(bool ok,int line) {
	++checks;
	if (!ok) { cerr<<"Adversarial failed line "<<line<<'\n'; abort(); }
}
#define ck(...) checkAt((__VA_ARGS__),__LINE__)
static bool near(ld a,ld b,ld e=1e-15L) {
	return abs(a-b)<=e*max({1.L,abs(a),abs(b)});
}
static W dist2(IP a,IP b) {
	W x=W(a.x)-b.x,y=W(a.y)-b.y; return x*x+y*y;
}
static void precisionBoundaries() {
	// Long-long coordinates near either endpoint, but every used
	// difference/product fits ll. Raw absolute-coordinate shoelace would
	// violate its documented contract, so it is intentionally not used.
	for (ll offset:{LLONG_MIN+1000,LLONG_MAX-1000,0LL}) {
		vector<IP> p{IP(offset,offset),IP(offset+12,offset),
			IP(offset+12,offset+5),IP(offset,offset+5),IP(offset+3,offset+1)};
		auto h=hull(p);
		ck(h.size()==4 && hullDiameter2(h)==169);
		ck(pointInConvex(h,IP(offset+6,offset+2))==1);
		ck(pointInPoly(h,IP(offset+13,offset+2))==-1);
		ck(pointInConvex(h,IP(offset,offset+3))==0);
		auto q=nearestPair(p);
		ck(q.first==10 && dist2(p[q.second.first],p[q.second.second])==10);
		ck(rectUnion({{offset,offset,offset+12,offset+5},
			{offset+3,offset+1,offset+10,offset+8}})==81);
	}
	const ll edge=2147483647;
	vector<IP> p{IP(0,0),IP(edge,edge)};
	ck(nearestPair(p).first==ll(W(edge)*edge*2));
	ck(hullDiameter2(p)==ll(W(edge)*edge*2));
	ck(rectUnion({{LLONG_MIN,0,-1,1}})==LLONG_MAX);
	ck(rectUnion({{0,0,LLONG_MAX,1},{0,0,LLONG_MAX,1}})==LLONG_MAX);
	ck(rectUnion({{0,0,LLONG_MAX,0},{0,0,0,LLONG_MAX}})==0);
}
static void transformedPolygons() {
	// Convex parallelogram under every orientation-preserving shear and
	// quarter-turn. Known inverse coordinates classify queries independently
	// of winding/parity. Extra edge/interior duplicates exercise hull cleanup.
	for (int run=0;run<600;++run) {
		ll k=ll(rng()%11)-5, w=2+ll(rng()%10), h=2+ll(rng()%10);
		ll ox=ll(rng()%200000001)-100000000,oy=ll(rng()%200000001)-100000000;
		int turn=int(rng()%4);
		auto tr=[&](ll x,ll y) {
			x+=k*y;
			for (int j=0;j<turn;++j) { ll t=x; x=-y; y=t; }
			return IP(x+ox,y+oy);
		};
		vector<IP> p{tr(0,0),tr(w,0),tr(w,h),tr(0,h)};
		vector<IP> cloud=p;
		for (int j=0;j<20;++j) cloud.push_back(tr(ll(rng()%uint64_t(w+1)),ll(rng()%uint64_t(h+1))));
		shuffle(cloud.begin(),cloud.end(),rng);
		auto ch=hull(cloud);
		ck(ch.size()==4 && polyArea2(p)==2*w*h && polyArea2(ch)==2*w*h);
		ll far=0;
		for (IP a:cloud) for (IP b:cloud) far=max(far,ll(dist2(a,b)));
		ck(hullDiameter2(ch)==far);
		for (int j=0;j<25;++j) {
			ll x=ll(rng()%uint64_t(w+5))-2,y=ll(rng()%uint64_t(h+5))-2;
			int want=x<0||x>w||y<0||y>h ? -1 : x==0||x==w||y==0||y==h ? 0 : 1;
			ck(pointInPoly(p,tr(x,y))==want && pointInConvex(ch,tr(x,y))==want);
		}
		// Clipping at an integer x-coordinate leaves a parallelogram of
		// known area; all operations are dyadic here and exactly representable.
		ll cut=w/2;
		vector<P2> rp;
		for (IP v:p) rp.push_back(toReal(v));
		auto out=convexCut(rp,toReal(tr(cut,0)),toReal(tr(cut,1)));
		ck(polyArea2(out)==2*cut*h);
		reverse(p.begin(),p.end());
		ck(polyArea2(p)==-2*w*h);
		++polygonCases;
	}
	// Collinear boundary vertices and repeated input vertices are retained
	// by clipping but area and membership remain correct.
	vector<P2> p{P2(0,0),P2(2,0),P2(4,0),P2(4,4),P2(0,4)};
	auto out=convexCut(p,P2(2,-1),P2(2,5));
	ck(polyArea2(out)==16);
	for (P2 a:out) ck(a.x<=2);
	ck(polyArea2(hull(out))==16);
}
static void analyticGeometry() {
	// Exactly represented right-angle transformations; tolerance grows only
	// with machine roundoff of the translation, not arbitrary test outcomes.
	for (ld scale:{1e-9L,1.L,1e9L}) for (int rot=0;rot<4;++rot)
		for (P2 shift:{P2(),P2(1e5L*scale,-1e5L*scale)}) {
			auto tr=[&](P2 p) {
				for (int i=0;i<rot;++i) p=p.perp();
				return shift+p*scale;
			};
			ld eps=256*numeric_limits<ld>::epsilon()*
				max({scale,abs(shift.x),abs(shift.y)});
			auto eq=[&](P2 a,P2 b) { return sqrtl((a-b).dist2())<=eps; };
			P2 a=tr(P2(0,0)),b=tr(P2(6,8)),q=tr(P2(-1,7));
			ck(eq(project(a,b,q),tr(P2(3,4))));
			ck(eq(reflect(a,b,q),tr(P2(7,1))));
			ck(abs(lineDist(a,b,q)-5*scale)<=eps);
			ck(abs(segDist(a,b,tr(P2(9,12)))-5*scale)<=eps);
			ck(abs(segDist(a,a,q)-sqrtl(50.L)*scale)<=eps);
			auto line=circleLine({tr(P2(0,0)),5*scale},tr(P2(-8,3)),tr(P2(8,3)),eps);
			ck(line.size()==2);
			ck((eq(line[0],tr(P2(-4,3))) && eq(line[1],tr(P2(4,3)))) ||
				(eq(line[1],tr(P2(-4,3))) && eq(line[0],tr(P2(4,3)))));
			auto cc=circleCircle({tr(P2(-3,0)),5*scale},{tr(P2(3,0)),5*scale},eps);
			ck(cc && cc->size()==2);
			ck((eq((*cc)[0],tr(P2(0,4))) && eq((*cc)[1],tr(P2(0,-4)))) ||
				(eq((*cc)[1],tr(P2(0,4))) && eq((*cc)[0],tr(P2(0,-4)))));
			vector<P2> triangle{tr(P2(-3,0)),tr(P2(3,0)),tr(P2(0,4))};
			auto circum=circumcircle(triangle[0],triangle[1],triangle[2]);
			ck(circum && eq(circum->c,tr(P2(0,0.875L))) && abs(circum->r-3.125L*scale)<=eps);
			for (int rep=0;rep<12;++rep) {
				auto mec=enclosingCircle(triangle,eps);
				ck(eq(mec.c,tr(P2(0,0.875L))) && abs(mec.r-3.125L*scale)<=eps);
			}
			++analyticCases;
		}
	// Tangency classification just inside/outside the chosen absolute eps.
	ld e=1e-6L;
	ck(circleLine({P2(),1},P2(-2,1+e/2),P2(2,1+e/2),e).size()==1);
	ck(circleLine({P2(),1},P2(-2,1+2*e),P2(2,1+2*e),e).empty());
	ck(circleLine({P2(),1},P2(-2,1-2*e),P2(2,1-2*e),e).size()==2);
	ck(circleCircle({P2(),1},{P2(2+e/2,0),1},e)->size()==1);
	ck(circleCircle({P2(),1},{P2(2+2*e,0),1},e)->empty());
	ck(circleCircle({P2(),1},{P2(2-2*e,0),1},e)->size()==2);
	ck(sgn(-4LL)==-1 && sgn(0LL)==0 && sgn(9LL)==1);
	ck(near(Point<ld>(3,4).unit().dist2(),1));
	ck(geoSgn(4LL,4LL)==0 && geoSgn(-5LL,4LL)==-1);
}
static void radiusGridNotes() {
	// Implements exactly the geometric recipe printed in the notes.
	// std::map is used instead of hashing so this test checks cell arithmetic
	// and neighbour coverage independently of a particular hash function.
	for (int run=0;run<1600;++run) {
		int n=int(rng()%36);
		ll r=ll(rng()%8),offset=run%7==0 ? LLONG_MIN+1000 :
			run%7==1 ? LLONG_MAX-1000 : 0;
		vector<IP> p;
		for (int j=0;j<n;++j) p.emplace_back(offset+ll(rng()%61)-30,offset+ll(rng()%61)-30);
		vector<pair<int,int>> want,got;
		for (int i=0;i<n;++i) for (int j=0;j<i;++j)
			if (dist2(p[i],p[j])<=W(r)*r) want.push_back({j,i});
		map<pair<ll,ll>,vec<int>> cells;
		for (int i=0;i<n;++i) {
			ll x=r ? floorDiv(p[i].x,r) : p[i].x;
			ll y=r ? floorDiv(p[i].y,r) : p[i].y;
			for (int dx=-(r!=0);dx<=int(r!=0);++dx)
				for (int dy=-(r!=0);dy<=int(r!=0);++dy) {
					auto it=cells.find({x+dx,y+dy});
					if (it!=cells.end()) for (int j:it->second)
						if (dist2(p[i],p[j])<=W(r)*r) got.push_back({j,i});
				}
			cells[{x,y}].push_back(i);
		}
		sort(want.begin(),want.end()); sort(got.begin(),got.end());
		ck(got==want); ++gridCases;
	}
}
int main() {
	precisionBoundaries(); transformedPolygons(); analyticGeometry(); radiusGridNotes();
	cout<<"Adversarial: "<<checks<<" checks, "<<polygonCases<<" transformed polygons, "
		<<analyticCases<<" analytical transformations, "<<gridCases
		<<" radius-grid cases; ll boundaries and floating tolerances passed\n";
}
