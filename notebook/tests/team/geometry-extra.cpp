#include "prelude.hpp"
#include "../../content/geometry/CircleTangents.h"
#include "../../content/geometry/CirclePolygonArea.h"
#include "../../content/geometry/PolygonCentroid.h"
#include "../../content/geometry/ManhattanMST.h"
#include "../../content/geometry/PolyArea.h"

static uint64_t checks = 0;
static void check(bool yes) { ++checks; assert(yes); }
static bool near(ld a,ld b,ld eps=1e-9L) {
    return abs(a-b) <= eps*max({ld(1),abs(a),abs(b)});
}
static ll mst(int n,vec<tuple<ll,int,int>> edges) {
    vec<int> parent(n),size(n,1);
    iota(all(parent),0);
    auto find = [&](int u) {
        while (parent[u]!=u) u=parent[u];
        return u;
    };
    sort(all(edges)); ll ans=0; int count=0;
    for (auto [w,u,v]:edges) {
        u=find(u);v=find(v);
        if(u==v)continue;
        if(size[u]<size[v])swap(u,v);
        parent[v]=u;size[u]+=size[v];ans+=w;++count;
    }
    check(count==max(0,n-1));
    return ans;
}

int main() {
    mt19937 rng(103911);
    const ld pi=acosl(-1);
    rep(it,0,1500) {
        int n=int(rng()%35);
        vec<Point<ll>> points;
        rep(i,0,n) points.pb(Point<ll>(ll(rng()%61)-30,ll(rng()%61)-30));
        auto sparse=manhattanEdges(points);
        check(sz(sparse)<=4*n);
        vec<tuple<ll,int,int>> complete;
        rep(i,0,n)rep(j,0,i)
            complete.pb({abs(points[i].x-points[j].x)+abs(points[i].y-points[j].y),i,j});
        check(mst(n,sparse)==mst(n,complete));
        for(auto [w,u,v]:sparse)check(w==abs(points[u].x-points[v].x)+abs(points[u].y-points[v].y));
    }
    vec<Point<ll>> large{Point<ll>(1000000000000000LL,-1000000000000000LL),
                        Point<ll>(-1000000000000000LL,1000000000000000LL)};
    check(mst(2,manhattanEdges(large))==4000000000000000LL);
    rep(it,0,1000) {
        ld x=int(rng()%100)-50,y=int(rng()%100)-50;
        ld w=1+rng()%20,h=1+rng()%20;
        vec<P2> p{P2(x,y),P2(x+w,y),P2(x+w,y+h),P2(x,y+h)};
        for(int orientation=0;orientation<2;++orientation) {
            auto c=polygonCentroid(p);
            check(c && near(c->x,x+w/2) && near(c->y,y+h/2));
            check(near(circlePolygonArea({P2(x,y),0},p),0));
            ld r=1+rng()%20;
            vec<P2> square{P2(-2*r,-2*r),P2(2*r,-2*r),P2(2*r,2*r),P2(-2*r,2*r)};
            check(near(circlePolygonArea({P2(0,0),r},square),pi*r*r));
            check(near(circlePolygonArea({P2(x+w/2,y+h/2),w+h},p),w*h));
            check(near(circlePolygonArea({P2(x-100,y-100),1},p),0));
            vec<P2> quarter{P2(0,0),P2(2*r,0),P2(2*r,2*r),P2(0,2*r)};
            check(near(circlePolygonArea({P2(0,0),r},quarter),pi*r*r/4));
            square.pb(square.back());square.pb(square.front());
            check(near(circlePolygonArea({P2(0,0),r},square),pi*r*r));
            reverse(all(p));
        }
    }
    check(!polygonCentroid({P2(0,0),P2(1,1),P2(2,2)}));
    // Disk/rectangle partial overlap, exact circular-segment formula.
    rep(i,1,100) {
        ld a=ld(i)/100;
        vec<P2> p{P2(-2,-2),P2(a,-2),P2(a,2),P2(-2,2)};
        ld expect=pi/2+asinl(a)+a*sqrtl(1-a*a);
        check(near(circlePolygonArea({P2(0,0),1},p),expect));
    }
    rep(it,0,2000) {
        ld r=1+rng()%10,s=1+rng()%10;
        Circle a{P2(int(rng()%21)-10,int(rng()%21)-10),r};
        Circle b{a.c+P2(30+rng()%20,20+rng()%20),s};
        for(bool inner:{false,true}) {
            auto ts=circleTangents(a,b,inner);
            check(sz(ts)==2);
            for(auto [p,q]:ts) {
                check(near((p-a.c).dist2(),r*r));
                check(near((q-b.c).dist2(),s*s));
                check(near((p-a.c).dot(q-p),0,1e-8L));
                check(near((q-b.c).dot(q-p),0,1e-8L));
            }
        }
    }
    check(circleTangents({P2(0,0),5},{P2(0,0),2}).empty());
    check(sz(circleTangents({P2(0,0),1},{P2(2,0),1},true))==1);
    check(sz(circleTangents({P2(0,0),1},{P2(3,0),0}))==2);
    cout<<"geometry-extra: "<<checks<<" checks passed\n";
}
