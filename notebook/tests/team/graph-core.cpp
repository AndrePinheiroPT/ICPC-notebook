#include "prelude.hpp"
#include "../../content/graph/Traversal.h"
#include "../../content/graph/GridBFS.h"
#include "../../content/graph/Dijkstra.h"
#include "../../content/graph/ZeroOneBFS.h"
#include "../../content/graph/BellmanFord.h"
#include "../../content/graph/FloydWarshall.h"
#include "../../content/graph/Kahn.h"
#include "../../content/graph/Kruskal.h"
#include "../../content/graph/StrongComponents.h"
#include "../../content/graph/Lowlink.h"
#include "../../content/graph/TwoSAT.h"
#include "../../content/graph/Hierholzer.h"
mt19937 rng(80321);
int rnd(int n){return int(rng()%unsigned(n));}
int components(int n,const vec<pair<int,int>>& e,int skipEdge=-1,int skipVertex=-1){
 vec<int> p(n);iota(all(p),0);auto find=[&](int u){while(u!=p[u])u=p[u];return u;};
 int ans=n-(skipVertex>=0);
 rep(i,0,sz(e)){auto [u,v]=e[i];if(i==skipEdge||u==skipVertex||v==skipVertex)continue;
  u=find(u);v=find(v);if(u!=v)p[u]=v,--ans;}
 return ans;
}
bool trailBrute(int v,int mask,const vec<pair<int,int>>& e,bool directed){
 if(mask==(1<<sz(e))-1)return true;
 rep(i,0,sz(e))if(!(mask>>i&1)){
  auto [a,b]=e[i];
  if(a==v&&trailBrute(b,mask|(1<<i),e,directed))return true;
  if(!directed&&b==v&&trailBrute(a,mask|(1<<i),e,directed))return true;
 }
 return false;
}
int main(){
 long long checks=0;
 rep(test,0,700){
  int n=1+rnd(8),m=rnd(25);
  vec<vec<int>> g(n);vec<vec<pair<int,ll>>> w(n);
  vec<vec<pair<int,int>>> z(n);vec<BF::Ed> ed;
  vec<vec<ll>> matrix(n,vec<ll>(n,FW::inf));
  rep(i,0,m){int u=rnd(n),v=rnd(n),c=rnd(13)-5;g[u].pb(v);ed.pb({u,v,c});
   matrix[u][v]=min(matrix[u][v],ll(c));w[u].pb({v,abs(c)});z[u].pb({v,abs(c)%2});}
  auto fw=matrix;FW::floydWarshall(fw);
  vec<vec<int>> reach(n,vec<int>(n));
  rep(s,0,n){reach[s][s]=1;for(auto e:ed)reach[e.a][e.b]=1;}
  rep(k,0,n)rep(i,0,n)rep(j,0,n)reach[i][j]|=reach[i][k]&&reach[k][j];
  rep(s,0,n){
   const ll BIG=1000000000000LL;vec<ll> d(n,BIG);d[s]=0;
   rep(k,0,n-1)for(auto e:ed)if(d[e.a]!=BIG)d[e.b]=min(d[e.b],d[e.a]+e.w);
   vec<int> neg(n);for(auto e:ed)if(d[e.a]!=BIG&&d[e.b]>d[e.a]+e.w)
    rep(v,0,n)if(reach[e.b][v])neg[v]=1;
   vec<BF::Node> nodes(n);auto edges=ed;BF::bellmanFord(nodes,edges,s);
   rep(v,0,n){assert(nodes[v].dist==(neg[v]?-BF::inf:d[v]==BIG?BF::inf:d[v]));
    assert(fw[s][v]==(neg[v]?-FW::inf:d[v]==BIG?FW::inf:d[v]));++checks;}
   auto [wd,p]=dijkstra(w,s);auto dense=dijkstraDense(w,s);
   assert(dense.first==wd);
   vec<ll> expected(n,LLONG_MAX/4);expected[s]=0;
   rep(k,0,n)rep(u,0,n)for(auto [v,c]:w[u])if(expected[u]!=LLONG_MAX/4)
    expected[v]=min(expected[v],expected[u]+c);
   assert(wd==expected);
   rep(v,0,n){auto path=restorePath(s,v,p);assert(path.empty()==(wd[v]==LLONG_MAX/4));
    if(!path.empty()){assert(sz(path)<=n);assert(path.front()==s&&path.back()==v);}}
  }
  vec<int> src;rep(i,0,rnd(n+2))src.pb(rnd(n));
  auto [d,p]=bfs(g,src);auto [zd,zp]=zeroOneBfs(z,src);auto [dd,dp]=dijkstra(w,src);
  vec<int> bd(n,-1),bz(n,INT_MAX);vec<ll> bw(n,LLONG_MAX/4);
  for(int s:src)bd[s]=bz[s]=0,bw[s]=0;
  rep(k,0,n)rep(u,0,n){
   for(int v:g[u])if(bd[u]>=0&&(bd[v]<0||bd[v]>bd[u]+1))bd[v]=bd[u]+1;
   for(auto [v,c]:z[u])if(bz[u]!=INT_MAX)bz[v]=min(bz[v],bz[u]+c);
   for(auto [v,c]:w[u])if(bw[u]!=LLONG_MAX/4)bw[v]=min(bw[v],bw[u]+c);
  }
  assert(d==bd&&zd==bz&&dd==bw);for(int s:src)assert(p[s]==-1&&zp[s]==-1&&dp[s]==-1);
  auto comp=kosaraju(g);auto dag=condensation(g,comp);
  rep(u,0,n)rep(v,0,n)assert((comp[u]==comp[v])==bool(reach[u][v]&&reach[v][u]));
  rep(u,0,n)for(int v:g[u])assert(comp[u]<=comp[v]);
  rep(c,0,sz(dag)){set<int> unique;for(int d:dag[c]){assert(c<d);assert(unique.insert(d).second);}}
  auto order=kahn(g);bool acyclic=true;for(auto e:ed)if(reach[e.b][e.a])acyclic=false;
  assert((sz(order)==n)==acyclic);if(acyclic){vec<int> pos(n);rep(i,0,n)pos[order[i]]=i;
   rep(u,0,n)for(int v:g[u])assert(pos[u]<pos[v]);}
 }
 rep(test,0,800){
  int n=1+rnd(7),m=rnd(9);vec<pair<int,int>> e;vec<vec<int>> g(n);
  rep(i,0,m){int a=rnd(n),b=rnd(n);e.pb({a,b});g[a].pb(b);g[b].pb(a);}
  auto [ok,c]=bipartite(g);bool expected=false;
  rep(mask,0,1<<n){bool good=true;for(auto [a,b]:e)if(((mask>>a)&1)==((mask>>b)&1))good=false;expected|=good;}
  assert(ok==expected);if(ok)for(auto [a,b]:e)assert(c[a]!=c[b]);
  Lowlink l(n,e);set<int> bridges(all(l.bridges));int base=components(n,e);
  rep(i,0,m)assert(bridges.count(i)==unsigned(components(n,e,i)>base));
  rep(v,0,n)assert(bool(l.cut[v])==(components(n,e,-1,v)>base));
  rep(directed,0,2){int s=rnd(n);auto [v,ids]=eulerTrail(n,e,s,directed);
   assert(!v.empty()==trailBrute(s,0,e,directed));if(!v.empty()){
    assert(sz(v)==m+1&&sz(ids)==m&&v[0]==s);set<int> used;
    rep(i,0,m){assert(used.insert(ids[i]).second);auto [a,b]=e[ids[i]];
     assert((a==v[i]&&b==v[i+1])||(!directed&&b==v[i]&&a==v[i+1]));}
   }}
  vec<tuple<int,int,ll>> weighted;for(auto [a,b]:e)weighted.pb({a,b,rnd(13)-6});
  auto [cost,ids]=kruskal(n,weighted);ll best=LLONG_MAX;
  rep(mask,0,1<<m)if(__builtin_popcount(unsigned(mask))==n-base){
   vec<pair<int,int>> pick;ll sum=0;rep(i,0,m)if(mask>>i&1){auto [a,b,c]=weighted[i];pick.pb({a,b});sum+=c;}
   if(components(n,pick)==base)best=min(best,sum);
  }
  assert(cost==best&&sz(ids)==n-base);++checks;
 }
 rep(test,0,1000){
  int n=1+rnd(7),m=rnd(30);TwoSAT sat(n);vec<pair<int,int>> clauses;
  rep(i,0,m){int a=rnd(2*n),b=rnd(2*n);sat.either(a,b);clauses.pb({a,b});}
  auto valid=[&](int mask){for(auto [a,b]:clauses){auto value=[&](int x){return ((mask>>(x/2))&1)^(x&1);};
   if(!value(a)&&!value(b))return false;}return true;};
  bool exists=false;rep(mask,0,1<<n)exists|=valid(mask);
  assert(sat.solve()==exists);if(exists){int mask=0;rep(i,0,n)mask|=sat.answer[i]<<i;assert(valid(mask));}++checks;
 }
 rep(test,0,300){int R=1+rnd(8),C=1+rnd(8);vec<string>a(R,string(C,'.'));
  rep(r,0,R)rep(c,0,C)if(rnd(4)==0)a[r][c]='#';
  vec<pair<int,int>> sources;
  rep(r,0,R)rep(c,0,C)if(a[r][c]!='#'&&rnd(5)==0)sources.pb({r,c});
  auto [d,p]=gridBfs(a,sources);vec<int> dist(R*C,-1);for(auto [r,c]:sources)dist[r*C+c]=0;
  rep(k,0,R*C)rep(r,0,R)rep(c,0,C)if(a[r][c]!='#'){
   for(auto [dr,dc]:vec<pair<int,int>>{{0,1},{0,-1},{1,0},{-1,0}}){int nr=r+dr,nc=c+dc;
    if(nr<0||nr>=R||nc<0||nc>=C||a[nr][nc]=='#'||dist[nr*C+nc]<0)continue;
    if(dist[r*C+c]<0||dist[r*C+c]>dist[nr*C+nc]+1)dist[r*C+c]=dist[nr*C+nc]+1;
   }}assert(d==dist);++checks;
 }
 {int n=100000;vec<vec<int>>g(n);rep(i,1,n)g[i-1].pb(i);auto c=kosaraju(g);assert(c.front()==0&&c.back()==n-1);}
 assert(kosaraju({}).empty());assert(condensation({},{}).empty());assert(bfs({},{}).first.empty());
 assert(kruskal(0,vec<tuple<int,int,ll>>{}).first==0);
 cout<<"graph-core: "<<checks<<" primary checks plus path/structure invariants passed\n";
}
