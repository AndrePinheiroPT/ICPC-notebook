#include "prelude.hpp"
#include "../../content/numerical/IntDeterminant.h"
// Independent validation of the graph chapter's two formula-only references.
mt19937 rng(3017);int rnd(int n){return int(rng()%unsigned(n));}
int main(){
 int checks=0;
 rep(test,0,700){int n=1+rnd(6),root=rnd(n);vec<pair<int,int>>edges;
  rep(i,0,rnd(15)){int a=rnd(n),b=rnd(n);edges.pb({a,b});}
  vec<vec<ll>>L(n,vec<ll>(n));vec<vec<int>>incoming(n);
  for(auto [a,b]:edges){--L[a][b];++L[b][b];incoming[b].pb(a);}
  vec<vec<ll>>minor(n-1,vec<ll>(n-1));
  rep(i,0,n)if(i!=root)rep(j,0,n)if(j!=root)minor[i-(i>root)][j-(j>root)]=L[i][j];
  vec<int>parent(n,-1);ll count=0;
  function<void(int)>dfs=[&](int v){if(v==n){rep(u,0,n){int x=u;rep(k,0,n)if(x!=root)x=parent[x];if(x!=root)return;}++count;return;}
   if(v==root)dfs(v+1);else for(int u:incoming[v]){parent[v]=u;dfs(v+1);}};
  dfs(0);for(ll mod:{1,6,17,1000000007})assert(detMod(minor,mod)==count%mod);++checks;
 }
 rep(n,0,6){vec<pair<int,int>>edges;rep(i,0,n)rep(j,i+1,n)edges.pb({i,j});set<vec<int>>possible;
  rep(mask,0,1<<sz(edges)){vec<int>d(n);rep(i,0,sz(edges))if(mask>>i&1)++d[edges[i].first],++d[edges[i].second];sort(d.rbegin(),d.rend());possible.insert(d);}
  vec<int>d(n);function<void(int,int)>enumerate=[&](int i,int lim){if(i<n){rep(v,0,lim+1){d[i]=v;enumerate(i+1,v);}return;}
   ll sum=accumulate(all(d),0LL);bool valid=sum%2==0;
   rep(k,1,n+1){ll left=0,right=ll(k)*(k-1);rep(j,0,k)left+=d[j];rep(j,k,n)right+=min(d[j],k);valid&=left<=right;}
   assert(valid==bool(possible.count(d)));++checks;
  };enumerate(0,n);
 }
 cout<<"graph-math: "<<checks<<" Matrix-Tree and degree-sequence checks passed\n";
}
