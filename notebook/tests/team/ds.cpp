#include "prelude.hpp"
#include "../../content/data-structures/ArrayBasics.h"
#include "../../content/data-structures/UnionFind.h"
#include "../../content/data-structures/UnionFindRollback.h"
#include "../../content/data-structures/DynamicConnectivity.h"
#include "../../content/data-structures/Fenwick.h"
#include "../../content/data-structures/Fenwick2D.h"
#include "../../content/data-structures/SegmentTree.h"
#include "../../content/data-structures/LazySegTree.h"
#include "../../content/data-structures/SparseLazySegTree.h"
#include "../../content/data-structures/RMQ.h"
#include "../../content/data-structures/SqrtDecomposition.h"
#include "../../content/data-structures/PrefixSum2D.h"
#include "../../content/data-structures/Difference2D.h"
#include "../../content/data-structures/OrderedSet.h"
#include "../../content/data-structures/HashMap.h"
#include "../../content/data-structures/Treap.h"
static long long checks=0;
#define CHECK(x) do {++checks; if (!(x)) {cerr<<"FAIL line "<<__LINE__<<" "<<#x<<endl; abort();}} while(0)
mt19937 rng(438473);
int rnd(int l,int r) {return uniform_int_distribution<int>(l,r)(rng);}
ll sum(const vec<ll>& a,int l,int r){return accumulate(a.begin()+l,a.begin()+r,0LL);}
ll mx(const vec<ll>& a,int l,int r){return l==r?LLONG_MIN:*max_element(a.begin()+l,a.begin()+r);}
int main(){
 rep(trial,0,180){
  int n=rnd(0,70); vec<ll>a(n),d(n+1),expected(n);
  for(ll&x:a)x=rnd(-50,50);
  auto p=prefixSums(a); CHECK(sz(p)==n+1 && p[0]==0);
  Fenwick<ll> f(n); SegTree st(n); SqrtDecomposition sq(a); LazySegTree lazy(a); RMQ rmq(a);
  rep(i,0,n){f.add(i,a[i]);st.update(i,a[i]);}
  rep(q,0,140){
   int l=rnd(0,n),r=rnd(l,n); ll x=rnd(-30,30);
   CHECK(rangeSum(p,l,r)==sum(a,l,r));
   if(l<r)CHECK(rmq.query(l,r)==*min_element(a.begin()+l,a.begin()+r));
   addRange(d,l,r,x); rep(i,l,r)expected[i]+=x;
  }
  CHECK(finishDiff(d)==expected);
  rep(q,0,150){
   int l=rnd(0,n),r=rnd(l,n); ll x=rnd(-50,50);
   CHECK(f.query(l,r)==sum(a,l,r)); CHECK(f.query(r)==sum(a,0,r)); CHECK(st.query(l,r)==mx(a,l,r));
   CHECK(sq.countLE(l,r,x)==count_if(a.begin()+l,a.begin()+r,[&](ll y){return y<=x;}));
   if(n){int i=rnd(0,n-1);f.add(i,x-a[i]);st.update(i,x);sq.set(i,x);a[i]=x;}
  }
  // Independent lazy oracle starts with the original array stored in prefix sums.
  rep(i,0,n)a[i]=p[i+1]-p[i];
  rep(q,0,220){
   int l=rnd(0,n),r=rnd(l,n); ll x=rnd(-50,50); int op=rnd(0,3);
   if(op==0){lazy.add(l,r,x);rep(i,l,r)a[i]+=x;}
   if(op==1){lazy.set(l,r,x);rep(i,l,r)a[i]=x;}
   CHECK(lazy.query(l,r)==sum(a,l,r)); CHECK(lazy.queryMax(l,r)==mx(a,l,r));
  }
  Fenwick<ll> positive(n); vec<ll>b(n);
  rep(i,0,n){b[i]=rnd(0,10);positive.add(i,b[i]);}
  rep(target,-2,int(sum(b,0,n))+3){
   int want=target<=0?-1:n;ll z=0;
   if(target>0)rep(i,0,n){z+=b[i];if(z>=target){want=i;break;}}
   CHECK(positive.lowerBound(target)==want);
  }
 }
 rep(trial,0,130){
  int n=rnd(1,80);SparseLazySegTree st(n);vec<ll>a(n);
  rep(q,0,300){int l=rnd(0,n),r=rnd(l,n);ll x=rnd(-40,40);int op=rnd(0,2);
   if(op==0){st.add(l,r,x);rep(i,l,r)a[i]+=x;}
   if(op==1){st.set(l,r,x);rep(i,l,r)a[i]=x;}
   CHECK(st.query(l,r)==mx(a,l,r));
  }
 }
 {SparseLazySegTree st(LLONG_MAX);st.set(0,LLONG_MAX,-4);st.add(LLONG_MAX-1,LLONG_MAX,10);
  CHECK(st.query(LLONG_MAX-1,LLONG_MAX)==6);CHECK(st.query(0,1)==-4);CHECK(st.query(0,LLONG_MAX)==6);}
 {SparseLazySegTree st(2);st.set(0,2,-LLONG_MAX);CHECK(st.query(0,1)==-LLONG_MAX);
  st.add(0,2,LLONG_MAX);st.add(0,2,1);CHECK(st.query(0,1)==1);CHECK(st.query(1,2)==1);}
 rep(trial,0,130){
  int n=rnd(0,12),m=rnd(0,12);vec<vec<ll>>a(n,vec<ll>(m));for(auto&v:a)for(ll&x:v)x=rnd(-10,10);
  PrefixSum2D p(n,m,a);Difference2D d(n,m);vec<vec<ll>>b(n,vec<ll>(m));
  vec<pair<ll,ll>> pts;rep(i,0,n)rep(j,0,m)pts.pb({i*3LL-15,j*4LL-20});
  Fenwick2D<ll,ll> f(pts);rep(i,0,n)rep(j,0,m)f.add(i*3LL-15,j*4LL-20,a[i][j]);
  rep(q,0,130){int r1=rnd(0,n),r2=rnd(r1,n),c1=rnd(0,m),c2=rnd(c1,m);ll z=0;
   rep(i,r1,r2)rep(j,c1,c2)z+=a[i][j];
   CHECK(p.query(r1,c1,r2,c2)==z);CHECK(f.query(r1*3LL-15,c1*4LL-20,r2*3LL-15,c2*4LL-20)==z);
   ll x=rnd(-7,7);d.add(r1,c1,r2,c2,x);rep(i,r1,r2)rep(j,c1,c2)b[i][j]+=x;
  }
  CHECK(d.values()==b);CHECK(d.values()==b);
 }
 rep(trial,0,100){
  int n=rnd(1,24);UF uf(n);RollbackUF rb(n);vec<pair<int,int>> edges;
  auto connected=[&](int a,int b){vec<int>seen(n);seen[a]=1;rep(k,0,n)for(auto[u,v]:edges)if(seen[u]||seen[v])seen[u]=seen[v]=1;return seen[b]!=0;};
  rep(q,0,70){int u=rnd(0,n-1),v=rnd(0,n-1);bool want=!connected(u,v);CHECK(uf.join(u,v)==want);CHECK(rb.join(u,v)==want);edges.pb({u,v});
   rep(i,0,n){CHECK(uf.sameSet(u,i)==connected(u,i));CHECK((rb.find(u)==rb.find(i))==connected(u,i));}
   int count=0;rep(i,0,n)count+=connected(u,i);CHECK(uf.size(u)==count);CHECK(rb.size(u)==count);
  }
  int snap=rb.time();rep(q,0,10)rb.join(rnd(0,n-1),rnd(0,n-1));rb.rollback(snap);CHECK(rb.time()==snap);
  rb.rollback(0);rep(i,0,n)CHECK(rb.size(i)==1);
 }
 rep(trial,0,100){
  int n=rnd(1,12),Q=rnd(0,25);DynamicConnectivity dc(n,Q);vec<tuple<int,int,int,int>>edges;
  rep(i,0,30){int l=rnd(0,Q),r=rnd(l,Q),u=rnd(0,n-1),v=rnd(0,n-1);dc.add(l,r,u,v);edges.pb({l,r,u,v});}
  int visited=0;auto run=[&](int tm,RollbackUF&uf){++visited;UF expected(n);for(auto[l,r,u,v]:edges)if(l<=tm&&tm<r)expected.join(u,v);
   rep(i,0,n)rep(j,0,n)CHECK((uf.find(i)==uf.find(j))==expected.sameSet(i,j));};
  dc.run(run);CHECK(visited==Q);dc.run(run);CHECK(visited==2*Q);rep(i,0,n)CHECK(dc.uf.size(i)==1);
 }
 rep(trial,0,120){
  ImplicitTreap t(trial);vec<ll>a;
  rep(q,0,350){int n=sz(a),l=rnd(0,n),r=rnd(l,n),op=rnd(0,5);
   if(op==0||n==0){ll x=rnd(-50,50);t.insert(l,x);a.insert(a.begin()+l,x);}
   else if(op==1){t.erase(l,r);a.erase(a.begin()+l,a.begin()+r);}
   else if(op==2){t.reverse(l,r);reverse(a.begin()+l,a.begin()+r);}
   else if(op==3){int k=rnd(0,n-(r-l));t.move(l,r,k);vec<ll>b(a.begin()+l,a.begin()+r);a.erase(a.begin()+l,a.begin()+r);a.insert(a.begin()+k,all(b));}
   else CHECK(t.query(l,r)==sum(a,l,r));
   CHECK(t.values()==a);CHECK(t.size()==sz(a));
  }
 }
 {OST<pair<ll,int>> s;HashMap<int>h;map<ll,int>ref;
 rep(i,0,300){ll x=rnd(-20,20);s.insert({x,i});++h[x];++ref[x];}
 vec<pair<ll,int>> a(all(s));rep(i,0,sz(a)){CHECK(*s.find_by_order(i)==a[i]);CHECK(s.order_of_key(a[i])==size_t(i));}
 CHECK(s.find_by_order(sz(a))==s.end());for(auto[x,c]:ref)CHECK(h[x]==c);}
 cout<<"ds: "<<checks<<" checks passed\n";
}
