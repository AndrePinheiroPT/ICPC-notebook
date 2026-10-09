#include "prelude.hpp"
#include "../../content/graph/JumpTable.h"
#include "../../content/graph/TreeTour.h"
#include "../../content/graph/TreeLCA.h"
#include "../../content/graph/HeavyLight.h"
#include "../../content/graph/TreeDiameter.h"
#include "../../content/graph/CompressTree.h"
#include "../../content/graph/CentroidDecomposition.h"
mt19937 rng(408115);int rnd(int n){return int(rng()%unsigned(n));}
int main(){
 long long checks=0;
 rep(test,0,600){int n=1+rnd(65),root=rnd(n);vec<vec<int>>g(n);vec<vec<pair<int,ll>>>wg(n);
  rep(v,1,n){int u=rnd(v);ll w=rnd(20);g[v].pb(u);g[u].pb(v);wg[u].pb({v,w});wg[v].pb({u,w});}
  TreeTour tour(g,root);TreeLCA t(g,root);HeavyLight h(g,root);CentroidDecomposition cd(g);
  vec<int>par(n,-1),dep(n),q{root};vec<ll>weighted(n);rep(i,0,sz(q)){int v=q[i];
   for(auto [u,w]:wg[v])if(u!=par[v])par[u]=v,dep[u]=dep[v]+1,weighted[u]=weighted[v]+w,q.pb(u);}
  assert(t.par==par&&h.par==par&&t.dep==dep&&h.dep==dep&&tour.par==par);
  auto naiveLCA=[&](int a,int b){while(dep[a]>dep[b])a=par[a];while(dep[b]>dep[a])b=par[b];while(a!=b)a=par[a],b=par[b];return a;};
  auto naiveDist=[&](int a,int b){int l=naiveLCA(a,b);return dep[a]+dep[b]-2*dep[l];};
  vec<ll>values(n);for(ll&v:values)v=rnd(100)-50;
  rep(v,0,n){assert(tour.ord[tour.tin[v]]==v&&h.ord[h.tin[v]]==v);
   rep(u,0,n){bool below=naiveLCA(u,v)==v;
    assert((tour.tin[v]<=tour.tin[u]&&tour.tin[u]<tour.tout[v])==below);
    assert((h.tin[v]<=h.tin[u]&&h.tin[u]<h.tout[v])==below);}
   int ancestor=v;rep(k,0,dep[v]+3){assert(t.ancestor(v,k)==ancestor);if(ancestor>=0)ancestor=par[ancestor];}
   assert(t.ancestor(v,ULLONG_MAX)==-1);
  }
  rep(i,0,100){int a=rnd(n),b=rnd(n),l=naiveLCA(a,b);assert(t.lca(a,b)==l&&t.dist(a,b)==naiveDist(a,b));
   vec<int>expected(n);for(int v=a;v!=l;v=par[v])expected[v]=1;for(int v=b;v!=l;v=par[v])expected[v]=1;expected[l]=1;
   vec<int>actual(n);h.path(a,b,[&](int left,int right){assert(0<=left&&left<right&&right<=n);rep(j,left,right)++actual[h.ord[j]];});assert(actual==expected);
   expected[l]=0;fill(all(actual),0);h.path(a,b,[&](int left,int right){rep(j,left,right)++actual[h.ord[j]];},true);assert(actual==expected);++checks;
  }
  auto [diameter,a,b]=treeDiameter(wg);ll best=0;
  rep(u,0,n)rep(v,0,n){int l=naiveLCA(u,v);best=max(best,weighted[u]+weighted[v]-2*weighted[l]);}
  assert(diameter==best&&weighted[a]+weighted[b]-2*weighted[naiveLCA(a,b)]==best);
  rep(i,0,10){vec<int>subset;rep(k,0,rnd(n+1))subset.pb(rnd(n));auto vt=compressTree(t,subset);
   set<int>closed(all(subset));for(int u:subset)for(int v:subset)closed.insert(naiveLCA(u,v));
   assert(sz(vt)==int(closed.size()));rep(k,0,sz(vt)){auto [p,v]=vt[k];assert(closed.count(v));assert((p==-1)==(k==0));
    if(p>=0){assert(p<k);int parent=vt[p].second;assert(naiveLCA(parent,v)==parent&&parent!=v);
     for(int u:closed)if(u!=v&&naiveLCA(u,v)==u)assert(dep[u]<=dep[parent]);}}
  }
  int centroidRoot=-1;rep(c,0,n)if(cd.parent[c]<0){assert(centroidRoot<0);centroidRoot=c;}
  vec<int>bestMarked(n,INT_MAX/2),marked(n);
  rep(v,0,n){assert(!cd.path[v].empty());assert(cd.path[v].front().first==centroidRoot&&cd.path[v].back()==make_pair(v,0));
   set<int>seen;for(auto [c,d]:cd.path[v]){assert(seen.insert(c).second);assert(d==naiveDist(v,c));}
   rep(i,1,sz(cd.path[v]))assert(cd.parent[cd.path[v][i].first]==cd.path[v][i-1].first);
   assert(sz(cd.path[v])<=1+int(log2(n)));}
  rep(i,0,n){int v=rnd(n);marked[v]=1;for(auto [c,d]:cd.path[v])bestMarked[c]=min(bestMarked[c],d);
   rep(u,0,n){int got=INT_MAX/2,expected=INT_MAX/2;for(auto [c,d]:cd.path[u])got=min(got,bestMarked[c]+d);
    rep(w,0,n)if(marked[w])expected=min(expected,naiveDist(u,w));
    assert(got==expected);}}
 }
 rep(test,0,800){int n=1+rnd(30);vec<int>next(n);for(int&v:next)v=rnd(n+1)-1;JumpTable jt(next,64);
  rep(i,0,50){int v=rnd(n),steps=rnd(500),expected=v;rep(k,0,steps)if(expected>=0)expected=next[expected];
   assert(jt.jump(v,steps)==expected);++checks;}}
 {JumpTable cycle(vec<int>{1,2,0},64);assert(cycle.jump(0,ULLONG_MAX)==int(ULLONG_MAX%3));}
 {int n=100000;vec<vec<int>>g(n);rep(i,1,n)g[i-1].pb(i),g[i].pb(i-1);
  TreeLCA t(g);assert(t.ancestor(n-1,n-1)==0&&t.ancestor(n-1,n)==-1);
  HeavyLight h(g);int calls=0;h.path(0,n-1,[&](int l,int r){assert(l==0&&r==n);++calls;});assert(calls==1);
  CentroidDecomposition c(g);assert(sz(c.path.back())<=18);}
 TreeTour empty({});TreeLCA emptyLCA({});HeavyLight emptyHLD({});CentroidDecomposition emptyCD({});
 assert(empty.ord.empty()&&emptyHLD.head.empty()&&emptyCD.parent.empty());
 assert(compressTree(emptyLCA,{}).empty());
 assert(treeDiameter(vec<vec<pair<int,ll>>>{})==make_tuple(0LL,-1,-1));
 cout<<"graph-trees: "<<checks<<" path/jump comparisons plus centroid, virtual tree and 100k-chain checks passed\n";
}
