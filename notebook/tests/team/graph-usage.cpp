// Literal current Usage snippets extracted from all 24 printed graph headers.
// Only the three intentionally contextual examples receive their missing g/u/v
// and a real default SegTree. Assertions check the stated outputs.
#include "prelude.hpp"
#include "../../content/data-structures/SegmentTree.h"
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
#include "../../content/graph/DinicFlow.h"
#include "../../content/graph/MinCostFlow.h"
#include "../../content/graph/BipartiteMatching.h"
#include "../../content/graph/WeightedMatching.h"
#include "../../content/graph/StableMatching.h"
#include "../../content/graph/JumpTable.h"
#include "../../content/graph/TreeTour.h"
#include "../../content/graph/TreeLCA.h"
#include "../../content/graph/HeavyLight.h"
#include "../../content/graph/TreeDiameter.h"
#include "../../content/graph/CompressTree.h"
#include "../../content/graph/CentroidDecomposition.h"
int main(){
 { // Traversal.h
  
 vec<vec<int>> g{{1},{0,2},{1}};
   auto [d,p] = bfs(g,{0}); // d={0,1,2}
   auto path = restorePath(0,2,p);
   auto [ok,color] = bipartite(g);
 
  assert((d==vec<int>{0,1,2}));assert((path==vec<int>{0,1,2}));assert(ok&&color[0]!=color[1]&&color[1]!=color[2]);
 }
 { // GridBFS.h
  
 vec<string> a{"...",".#.","..."};
   auto [d,p] = gridBfs(a,{{0,0}});
   // d[2*3+2]=4; p contains the path.
 
  assert(d[2*3+2]==4);assert(p[0]==-1);auto path=restorePath(0,8,p);assert(sz(path)==5);
 }
 { // Dijkstra.h
  
 vec<vec<pair<int,ll>>> g(3);
   g[0]={{1,7}}; g[1]={{2,2}};
   auto [d,p] = dijkstra(g,0); // {0,7,9}
 
  assert((d==vec<ll>{0,7,9}));assert((restorePath(0,2,p)==vec<int>{0,1,2}));
 }
 { // ZeroOneBFS.h
  
 vec<vec<pair<int,int>>> g(3);
   g[0]={{1,0},{2,1}}; g[1]={{2,0}};
   auto [d,p] = zeroOneBfs(g,{0}); // {0,0,0}
 
  assert((d==vec<int>{0,0,0}));assert((restorePath(0,2,p)==vec<int>{0,1,2}));
 }
 { // BellmanFord.h
  
 vec<BF::Node> nodes(3);
   vec<BF::Ed> edges{{0,1,4},{1,2,-2}};
   BF::bellmanFord(nodes,edges,0);
   ll answer=nodes[2].dist; // 2
 
  assert(answer==2&&nodes[2].prev==1);
 }
 { // FloydWarshall.h
  
 vec<vec<ll>> d(3,vec<ll>(3,FW::inf));
   d[0][1]=4; d[1][2]=-2;
   FW::floydWarshall(d); // d[0][2]=2
 
  assert(d[0][2]==2&&d[2][0]==FW::inf);
 }
 { // Kahn.h
  
 vec<vec<int>> g{{2},{2},{}};
   auto order=kahn(g); // {0,1,2}
   bool dag=sz(order)==sz(g);
 
  assert(dag&&(order==vec<int>{0,1,2}));
 }
 { // Kruskal.h
  
 vec<tuple<int,int,ll>> e{{0,1,4},{1,2,2},{0,2,9}};
   auto [cost,ids]=kruskal(3,e); // cost=6
 
  assert(cost==6&&(ids==vec<int>{1,0}));
 }
 { // StrongComponents.h
  
 vec<vec<int>> g{{1},{0,2},{3},{2}};
   auto comp=kosaraju(g); // {0,0,1,1}
   auto dag=condensation(g,comp); // {{1},{}}
 
  assert((comp==vec<int>{0,0,1,1}));assert((dag==vec<vec<int>>{{1},{}}));
 }
 { // Lowlink.h
  
 vec<pair<int,int>> e{{0,1},{1,2},{2,0},{1,3}};
   Lowlink b(4,e); // bridges={3}, cut[1]=1
 
  assert((b.bridges==vec<int>{3}));assert((b.cut==vec<int>{0,1,0,0}));
 }
 { // TwoSAT.h
  
 TwoSAT s(2);
   s.either(0,2); s.either(1,3); // exactly one
   s.either(0,0);
   bool ok=s.solve(); // answer={1,0}
 
  assert(ok&&(s.answer==vec<int>{1,0}));
 }
 { // Hierholzer.h
  
 vec<pair<int,int>> e{{0,1},{1,2}};
   auto [v,ids]=eulerTrail(3,e,0,false);
   // v={0,1,2}, ids={0,1}
 
  assert((v==vec<int>{0,1,2}));assert((ids==vec<int>{0,1}));
 }
 { // DinicFlow.h
  
 DinicFlow f(3);
   auto edge=f.add(0,1,5); f.add(1,2,3);
   ll sent=f.flow(0,2); // 3
   auto side=f.minCut(0); // {1,1,0}
 
  assert(sent==3&&(side==vec<int>{1,1,0}));assert(5-f.g[edge.first][edge.second].cap==3);
 }
 { // MinCostFlow.h
  
 MinCostFlow f(3);
   f.add(0,1,2,3); f.add(1,2,2,1);
   auto [sent,cost]=f.flow(0,2,2); // {2,8}
 
  assert(sent==2&&cost==8);
 }
 { // BipartiteMatching.h
  
 BipartiteMatching b(2,3);
   b.add(0,1); b.add(1,1); b.add(1,2);
   int pairs=b.solve(); // 2
   auto [left,right]=b.cover();
 
  assert(pairs==2&&sz(left)+sz(right)==2);assert(b.l[0]==1&&b.l[1]==2);
 }
 { // WeightedMatching.h
  
 vec<vec<ll>> cost{{4,1,9},{2,8,3}};
  auto [total,match]=weightedMatching(cost);
  // total=3, match={1,0}
 
  assert(total==3&&(match==vec<int>{1,0}));
 }
 { // StableMatching.h
  
 vec<vec<int>> a{{0,1},{0}}, b{{1,0},{0}};
  auto partner=stableMatching(a,b); // {1,0}
 
  assert((partner==vec<int>{1,0}));
 }
 { // JumpTable.h
  
 JumpTable t(vec<int>{1,2,0},64);
   int v=t.jump(0,5); // 2
 
  assert(v==2);
 }
 { // TreeTour.h
  
 vec<vec<int>> g{{1},{0,2,3},{1},{1}};
   TreeTour t(g);
   // subtree(1): [t.tin[1],t.tout[1]) = [1,4)
 
  assert(t.par[0]==-1&&t.tin[1]==1&&t.tout[1]==4);
 }
 { // TreeLCA.h
  
 vec<vec<int>> g{{1},{0,2,3},{1},{1}};
   TreeLCA t(g);
   int c=t.lca(2,3); // 1
   int a=t.ancestor(2,3); // -1
 
  assert(c==1&&a==-1);
 }
 { // HeavyLight.h
  vec<vec<int>> g{{1},{0,2,3},{1},{1}};int u=2,v=3;SegTree seg(4);rep(i,0,4)seg.update(i,i+1);
 HeavyLight h(g,0);
   ll ans=SegTree::unit;
   h.path(u,v,[&](int l,int r){ans=max(ans,seg.query(l,r));});
 
  assert(ans==max({ll(h.tin[1]+1),ll(h.tin[2]+1),ll(h.tin[3]+1)}));
 }
 { // TreeDiameter.h
  
 vec<vec<pair<int,ll>>> g{
       {{1,4}},{{0,4},{2,2}},{{1,2}}};
   auto [length,a,b]=treeDiameter(g); // length=6
 
  assert(length==6&&((a==0&&b==2)||(a==2&&b==0)));
 }
 { // CompressTree.h
  vec<vec<int>> g{{1},{0,2,3},{1},{1}};int u=2,v=3;
 TreeLCA t(g);
  auto vt=compressTree(t,{u,v});
  // vt[i].first is an index, vt[i].second an original ID.
 
  assert(sz(vt)==3&&vt[0]==make_pair(-1,1));set<int> originals;for(auto [p,v]:vt)originals.insert(v);assert((originals==set<int>{1,2,3}));
 }
 { // CentroidDecomposition.h
  vec<vec<int>> g{{1},{0,2,3},{1},{1}};int u=2,v=3;
 CentroidDecomposition cd(g);
  vec<int> best(sz(g),INT_MAX/2);
  for (auto [c,d]:cd.path[v]) best[c]=min(best[c],d);
  int ans=INT_MAX/2;
  for (auto [c,d]:cd.path[u]) ans=min(ans,best[c]+d);
 
  assert(ans==2);
 }
 cout<<"graph-usage: 24 literal printed examples compiled and passed\n";
}
