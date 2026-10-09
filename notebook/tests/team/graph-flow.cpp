#include "prelude.hpp"
#include "../../content/graph/DinicFlow.h"
#include "../../content/graph/MinCostFlow.h"
#include "../../content/graph/BipartiteMatching.h"
#include "../../content/graph/StableMatching.h"
#include "../../content/graph/WeightedMatching.h"
mt19937 rng(299702);int rnd(int n){return int(rng()%unsigned(n));}
int bruteMatching(const vec<vec<int>>&g,int u,int mask){
 if(u==sz(g))return 0;
 int ans=bruteMatching(g,u+1,mask);
 for(int v:g[u])if(!(mask>>v&1))ans=max(ans,1+bruteMatching(g,u+1,mask|(1<<v)));
 return ans;
}
int main(){
 int checks=0;
 rep(test,0,1200){int n=rnd(7),m=rnd(7);BipartiteMatching b(n,m);
  rep(u,0,n)rep(v,0,m)if(rnd(2))b.add(u,v);
  int count=b.solve();assert(count==bruteMatching(b.g,0,0));
  assert(b.solve()==count);int actual=0;
  rep(u,0,n)if(b.l[u]>=0){assert(b.r[b.l[u]]==u);assert(find(all(b.g[u]),b.l[u])!=b.g[u].end());++actual;}
  assert(actual==count);auto [l,r]=b.cover();assert(sz(l)+sz(r)==count);
  vec<int> a(n),c(m);for(int u:l)a[u]=1;for(int v:r)c[v]=1;
  rep(u,0,n)for(int v:b.g[u])assert(a[u]||c[v]);
  ++checks;
 }
 rep(test,0,650){int n=rnd(5),m=rnd(5);vec<vec<int>>a(n),b(m);
  rep(u,0,n){rep(v,0,m)if(rnd(2))a[u].pb(v);shuffle(all(a[u]),rng);}
  rep(v,0,m){rep(u,0,n)if(rnd(2))b[v].pb(u);shuffle(all(b[v]),rng);}
  auto partner=stableMatching(a,b);
  vec<vec<int>>ra(n,vec<int>(m,99)),rb(m,vec<int>(n,99));
  rep(u,0,n)rep(i,0,sz(a[u]))ra[u][a[u][i]]=i;
  rep(v,0,m)rep(i,0,sz(b[v]))rb[v][b[v][i]]=i;
  auto stable=[&](const vec<int>&p){vec<int>r(m,-1);
   rep(u,0,n)if(p[u]>=0){int v=p[u];if(r[v]>=0||ra[u][v]==99||rb[v][u]==99)return false;r[v]=u;}
   rep(u,0,n)rep(v,0,m)if(ra[u][v]<99&&rb[v][u]<99)
    if((p[u]<0||ra[u][v]<ra[u][p[u]])&&(r[v]<0||rb[v][u]<rb[v][r[v]]))return false;
   return true;};
  assert(stable(partner));vec<int>p(n,-1);
  function<void(int,int)>enumerate=[&](int u,int mask){if(u==n){if(stable(p))rep(i,0,n){
    int got=partner[i]<0?99:ra[i][partner[i]],alt=p[i]<0?99:ra[i][p[i]];assert(got<=alt);}return;}
   p[u]=-1;enumerate(u+1,mask);
   rep(v,0,m)if(!(mask>>v&1)){p[u]=v;enumerate(u+1,mask|(1<<v));}
   p[u]=-1;
  };enumerate(0,0);++checks;
 }
 rep(test,0,500){int n=rnd(6),m=n+rnd(3);vec<vec<ll>>c(n,vec<ll>(m));
  for(auto&row:c)for(ll&w:row)w=rnd(31)-15;
  ll best=LLONG_MAX;function<void(int,int,ll)>dfs=[&](int u,int mask,ll cost){
   if(u==n){best=min(best,cost);return;}rep(v,0,m)if(!(mask>>v&1))dfs(u+1,mask|(1<<v),cost+c[u][v]);};
  dfs(0,0,0);auto [cost,match]=weightedMatching(c);assert(cost==best);ll sum=0;set<int>used;
  rep(u,0,n){assert(used.insert(match[u]).second);sum+=c[u][match[u]];}assert(sum==cost);++checks;
 }
 rep(test,0,1000){int n=2+rnd(7),m=rnd(28),s=0,t=n-1;DinicFlow f(n);
  vec<tuple<int,int,ll>>edges;vec<pair<int,int>>handles;
  rep(i,0,m){int u=rnd(n),v=rnd(n);ll c=rnd(10);handles.pb(f.add(u,v,c));edges.pb({u,v,c});}
  ll best=LLONG_MAX;
  rep(mask,0,1<<n)if((mask>>s&1)&&!(mask>>t&1)){ll cost=0;
   for(auto [u,v,c]:edges)if((mask>>u&1)&&!(mask>>v&1))cost+=c;
   best=min(best,cost);}
  assert(f.flow(s,t)==best);auto side=f.minCut(s);assert(side[s]&&!side[t]);
  ll cut=0;vec<ll>net(n);rep(i,0,m){auto [u,v,c]=edges[i];auto [a,j]=handles[i];
   ll sent=c-f.g[a][j].cap;assert(0<=sent&&sent<=c);net[u]-=sent;net[v]+=sent;
   if(side[u]&&!side[v])cut+=c;}
  assert(cut==best&&net[s]==-best&&net[t]==best);rep(v,1,n-1)assert(net[v]==0);
  assert(f.flow(s,t)==0);++checks;
 }
 rep(test,0,350){int n=2+rnd(4),m=rnd(8);vec<tuple<int,int,ll,ll>>edges;MinCostFlow f(n);
  vec<int>pot(n);for(auto&x:pot)x=rnd(9)-4;
  rep(i,0,m){int u=rnd(n),v=rnd(n);ll cap=rnd(3),cost=rnd(6)+pot[v]-pot[u];
   edges.pb({u,v,cap,cost});f.add(u,v,cap,cost);}
  // Reduced edge costs are nonnegative: no negative cycle, even with loops.
  int limit=rnd(5);ll bestFlow=-1,bestCost=LLONG_MAX;vec<ll>balance(n);
  function<void(int,ll)>dfs=[&](int i,ll cost){if(i==m){
    rep(v,1,n-1)if(balance[v])return;
    ll flow=balance[n-1];
    if(flow<0||flow>limit||balance[0]!=-flow)return;
    if(flow>bestFlow||(flow==bestFlow&&cost<bestCost))bestFlow=flow,bestCost=cost;
    return;}
   auto [u,v,cap,w]=edges[i];rep(sent,0,int(cap)+1){balance[u]-=sent;balance[v]+=sent;
    dfs(i+1,cost+sent*w);balance[u]+=sent;balance[v]-=sent;}
  };dfs(0,0);
  auto res=f.flow(0,n-1,limit);assert(res==make_pair(bestFlow,bestCost));
  MinCostFlow incremental(n);for(auto [u,v,c,w]:edges)incremental.add(u,v,c,w);
  auto one=incremental.flow(0,n-1,min(limit,1));auto rest=incremental.flow(0,n-1,limit-one.first);
  assert(one.first+rest.first==bestFlow&&one.second+rest.second==bestCost);++checks;
 }
 {DinicFlow f(2);auto id=f.add(0,1,5000000000LL);assert(f.flow(0,1)==5000000000LL);assert(f.g[id.first][id.second].cap==0);}
 {MinCostFlow f(2);f.add(0,1,2000000000LL,1000000000LL);assert(f.flow(0,1)==make_pair(2000000000LL,2000000000000000000LL));}
 cout<<"graph-flow: "<<checks<<" random/exhaustive comparisons passed\n";
}
