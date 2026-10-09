# Graph selection and validation

## Selection from the active printed input chapters

Sources: **Gonçalo/H** (18 active graph headers), **Jet** (18 active imports; 0–1 BFS commented out), **André** (21 active imports, plus two formula references). Active imports were inspected, rather than interpreting every inherited KACTL header as printed content.

The team chapter has **24 active headers**. It preserves all 18 active graph techniques from H, restores Jet's matching/stable matching/HLD, and incorporates André's assignment/virtual-tree/centroid topics. Definitions use `vec<T>`, `rep`, `pb`, zero-based IDs, `[l,r)`, roots with parent `-1`, and `jmp[level][vertex]`.

| Input topic | Selected version and reason |
|---|---|
| BFS, path reconstruction, bipartite colouring (H) | Keep H multi-source distances+parents and iterative colouring of every component. |
| Grid BFS (H) | Keep multi-source grid interface and flattened parent IDs. |
| Dijkstra (all; André also dense) | H heap core with strict parent improvements, `ll`, guarded addition; add multi-source overload. Preserve André's meaningful dense `O(V²+E)` alternative as `dijkstraDense`. |
| 0–1 BFS (H, André; Jet reserve) | Keep H multi-source distances+parents, stale-entry deque handling. André snippet had duplicate declarations; not copied. |
| Bellman–Ford (all) | **Preserve H/B's sorted-edge relaxation and negative-infinity propagation algorithm**. Only aliases, loop macro and documentation change. |
| Floyd–Warshall (all) | Preserve negative-cycle propagation and bounded `ll` arithmetic in `FW`. |
| Topological order (all) | Kahn, including partial-result/cycle contract. |
| Kruskal (H/Jet) | Preserve weight template, original edge IDs, cost and disconnected-forest semantics; depends on shared `UF`. |
| SCC (all) | H iterative Kosaraju and optional condensation helper; ascending component IDs along DAG edges. No recursive regression. |
| Bridges/articulation points (H/Jet) | H iterative Lowlink with parent edge IDs, supporting loops and parallel edges. |
| 2-SAT (all) | H `TwoSAT` using the selected SCC ordering, literals `2*v`, `2*v+1`. |
| Euler trail (all) | H iterative version, directed/undirected support, degree/connectivity validation, both vertex and edge-ID sequences. |
| Max flow (all) | H/Jet Dinic; augment `add` with a returned edge handle and expose `minCut(s)` explicitly. Existing callers can ignore the return value. |
| Min-cost max-flow (all) | H/Jet potentials+Dijkstra version supporting negative edges, bounded additional flow and repeated calls. No negative residual cycles permitted. |
| Hopcroft–Karp, DFS matching, minimum vertex cover (André); matching+cover (Jet) | Jet `BipartiteMatching` unifies maximum matching, both partner arrays, minimum vertex cover, and independent-set complements. Separate DFS matching is redundant for intended use. |
| Weighted assignment (André) | `WeightedMatching.h`, André's rectangular shortest-augmenting-path assignment, now const input and `ll` costs. Retains `O(n²m)`, `n<=m`. |
| Stable matching (Jet) | Gale–Shapley extended to unequal sides and incomplete mutually acceptable preference lists. Keeps proposer-partner return vector; `-1` means unmatched. |
| Binary lifting/LCA (all) | H `JumpTable`, `TreeTour`, `TreeLCA`; keeps iterative tree traversal, absent ancestor `-1`, and 64-bit successor jumps. André's Euler-tour/RMQ LCA is not another printed default. |
| HLD (Jet; André commented) | Jet iterative `HeavyLight`, callback intervals `[l,r)`, vertex/edge modes, subtree intervals; document noncommutative limitation. |
| Tree diameter (H/Jet) | H generic nonnegative-weight iterative traversal returning length and endpoints. |
| Virtual tree / CompressTree (André) | Reworked around shared `TreeLCA`; no static array; handles empty/duplicate subsets; returns `(compressedParent,originalVertex)` with root parent `-1`. |
| Centroid decomposition (André) | Replace untested 1-based recursive placeholder with iterative 0-based decomposition exposing centroid parents and ancestor-distance lists. Include a small nearest-activated-vertex recipe. |
| Matrix–Tree and Erdős–Gallai (André) | Preserve compact mathematical references. Clarify the directed determinant counts out-arborescences; use shared `detMod` for the determinant. |

## Explicit reserve / non-print decisions

- **Link–cut tree (André active):** retained as source reserve, not printed. Its distinct use is online link/cut in changing forests; HLD only handles a fixed tree, so this is a conscious specialist-coverage exclusion, not an equivalent replacement. Pointer/splay complexity and weak original test status make it poor use of the strict page budget.
- **Directed MST/arborescence (André active):** retained as source reserve, not printed. Directed spanning optimisation is distinct from Kruskal and from Matrix–Tree counting. Skew heaps, rollback and reconstruction are substantial for a rare specialist use; no claim that MIUP can never require it.
- **Separate DFS matching, Euler/RMQ LCA, old traversal/flow/SCC variants:** no duplicate printouts when the selected interface already covers the same requested outputs. Original files remain available in source reserve.
- The many other inherited KACTL graph headers were already **inactive in the supplied chapters** (push-relabel, Edmonds–Karp, global min-cut, Gomory–Hu, blossom/general matching, clique/colouring heuristics, biconnected components). They were not represented as active input coverage or silently deleted.
- Removed H's long exercise-based GraphGuide from the printed chapter. Replaced by short graph modelling, flow objective, tree-DP and formula notes. The team chapter is a reference rather than a training worksheet.

## Contracts and risks documented

- BFS/grid unreachable distance `-1`; 0–1 BFS `INT_MAX`; Dijkstra custom `inf`; Bellman–Ford/Floyd distinguish `+inf` and negative-cycle `-inf`. Parents use `-1` at roots/unreachable vertices; Bellman parents only meaningful for finite distances.
- Trees must be connected undirected trees. Diameter needs nonnegative weights. HLD's callback pieces are unordered and require a commutative aggregate unless direction handling is added.
- Dinic and Hopcroft–Karp retain recursive augmenting DFS (depth `O(V)`), explicitly documented. SCC/Lowlink/tree preprocessing/centroid are iterative and tested on long chains.
- Flow capacities/cost products and all intermediate sums must fit `ll`; min-cost flow prohibits negative residual cycles. Assignment requires a complete rectangular cost matrix (finite penalty+verification for missing edges).
- Stable preferences have no ties and no duplicate IDs; mutually unacceptable pairs are excluded.

## Tests implemented and run

All programs include the shared `tests/team/prelude.hpp`. Each was compiled under C++17 in release (`-O2`) and ASan+UBSan (`-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -D_GLIBCXX_ASSERTIONS`). LeakSanitizer is disabled with `ASAN_OPTIONS=detect_leaks=0`: this execution sandbox uses ptrace, under which LSAN itself aborts. Address and undefined-behaviour instrumentation remain enabled.

- **graph-core.cpp:** 19,481 primary checks plus invariants. Bellman/Floyd against textbook relaxation with independently propagated negative reachability; Dijkstra/0–1/BFS against relaxation; parents and multi-source contracts; SCC against transitive closure; topological ordering; exhaustive bipartite colouring, bridges/articulations by deletion, tiny Euler-trail enumeration, spanning-forest enumeration, 2-SAT truth tables, grid oracle; 100,000-vertex SCC chain.
- **graph-flow.cpp:** 3,700 random/exhaustive comparisons. Matching against enumeration and cover/partner invariants; stable matching against all small stable partial assignments and proposer-optimality; weighted assignment against permutations; Dinic against every source/sink cut plus flow conservation; min-cost flow against bounded enumeration including loops/cycles, negative edge costs without negative cycles, and repeated-call results. Large `ll` capacities/cost products included.
- **graph-trees.cpp:** 100,000 named path/jump comparisons plus subtree/LCA invariants, vertex/edge HLD coverage, weighted diameter brute force, virtual-tree closure/nearest virtual parents, centroid paths/nearest active query oracle, 64-bit jump extremes. TreeLCA/HLD/centroid process a 100,000-vertex chain.
- **graph-math.cpp:** 1,051 checks. Matrix–Tree directed formula versus enumerated rooted arborescences, modulo prime and composite numbers; Erdős–Gallai versus all simple graphs up to five vertices and all sorted bounded degree sequences.
- **graph-kactl.cpp:** 1,580 stress cases adapted from the supplied KACTL `stress-tests/graph/WeightedMatching.cpp`, comparing rectangular assignment to unit-capacity min-cost flow. Uses current APIs and removes the legacy test's premature return after its first iteration.

The reserve LCT/DMST implementations are not newly validated or claimed to follow the active team's interface conventions. The full notebook runner handles isolated/combined inclusion and final integration with other chapters.
