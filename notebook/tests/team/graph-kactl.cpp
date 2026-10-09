#include "prelude.hpp"
#include "../../content/graph/WeightedMatching.h"
#include "../../content/graph/MinCostFlow.h"
// Adapted from supplied KACTL stress-tests/graph/WeightedMatching.cpp:
// match every left vertex and compare its cost with a unit-capacity flow.
// Uses current names/types, deterministic seed, and runs EVERY iteration
// (the supplied legacy test returned after its first iteration).
mt19937 rng(192013);
int main(){
 int checks=0;
 for(auto [N,mxCost,iters]:vec<tuple<int,int,int>>{{25,5,500},{70,1000,40},{70,1,40},{5,5,1000}}){
  rep(it,0,iters){int n=int(rng()%unsigned(N)),m=int(rng()%unsigned(N));if(n>m)swap(n,m);
   MinCostFlow f(n+m+2);int s=n+m,t=s+1;
   rep(u,0,n)f.add(s,u,1,0);
   rep(v,0,m)f.add(n+v,t,1,0);
   vec<vec<ll>>cost(n,vec<ll>(m));
   rep(u,0,n)rep(v,0,m){cost[u][v]=int(rng()%unsigned(2*mxCost+1))-mxCost;f.add(u,n+v,1,cost[u][v]);}
   auto [sent,total]=f.flow(s,t);auto [answer,match]=weightedMatching(cost);
   assert(sent==n&&total==answer);ll sum=0;set<int>used;
   rep(u,0,n){sum+=cost[u][match[u]];assert(used.insert(match[u]).second);}assert(sum==answer);++checks;
  }
 }
 cout<<"graph-kactl: "<<checks<<" adapted matching-vs-flow stress cases passed\n";
}
