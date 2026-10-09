# Three-notebook selection map

This report maps the **actual printed inputs** into the team encyclopedia. It follows each input `content/kactl.tex`, then its uncommented chapter imports and text includes. A filename existing in an archive is not evidence that it was printed.

The current team has **117 canonical algorithm headers + 3 contest setup imports**. The detailed [selection.json](selection.json) contains every active input import, its SHA-256, target canonical path(s), classification and rationale; it also lists text includes and headers not directly printed, distinguishing code dependencies.

## Coverage accounting

| Input | Active header imports | Setup/checklist imports | Canonical path | Consolidated/renamed | Formula only | Specialist reserve | Guide removed |
|---|---:|---:|---:|---:|---:|---:|---:|
| Gonçalo H | 65 | 3 | 65 | 1 | 0 | 0 | 2 |
| Jet | 87 | 4 | 85 | 5 | 0 | 0 | 1 |
| André | 100 | 4 | 28 | 62 | 1 | 12 | 1 |

Counts classify import **occurrences**, not distinct algorithms: one target can replace several source imports, and one source import can cover multiple techniques. “Canonical path” does not mean that the source was copied unchanged.

## Canonical families

| Area | Team selection | Source contribution |
|---|---|---|
| Contest/setup | Shared C++17 template, compile/debug configuration, troubleshooting and STL reference | H layout and practical setup; common team aliases; original setup variants consolidated. |
| Arrays/DS | Prefix/difference 1D/2D, Fenwick 1D/2D, segment trees, dense and sparse lazy, minimum RMQ, sqrt, UF/rollback/dynamic connectivity, OST, hash map, implicit treap | H simple contracts; Jet 2D/rollback interfaces; André large-domain lazy, sequence treap and hashed maps. |
| Search/math | Integer/real/ternary search, exact arithmetic, probability, modular matrices, three Gauss variants, determinants/inverse, interpolation, recurrence, NTT/FFT | H half-open optional binary search and safe arithmetic; Jet core numeric routines; André specialised modular/polynomial capabilities with explicit contracts. |
| Number theory/counting | Inverse/general CRT, SPF/trial/MR/Pollard, divisors, discrete log/root, floor sums, phi/Mobius, binomial/modular combinations, permutation cycles and counting identities | Restores advanced Jet coverage; integrates selected André unique routines without importing duplicate global-mod frameworks. |
| Graphs | Traversals/shortest paths incl. dense Dijkstra, SCC/2-SAT/lowlink/Euler, MST, max/min-cost flow, bipartite matching/cover, assignment and stable matching | H iterative graph safety and parents; Jet matching/stability; André dense shortest path and rectangular assignment. |
| Trees | Jump/LCA/tour/diameter, HLD, virtual tree, centroid decomposition | H parent/jump conventions; Jet iterative HLD; André virtual-tree and centroid capabilities rewritten around shared interfaces. |
| Geometry | Exact predicates, intersections/distances, polygon/hull/circle methods, closest pair, rectangle union, tangents, disk-polygon area, centroid and Manhattan MST | H/Jet Point interface; Jet restored geometry; André unique capabilities adapted rather than mixed incompatible point types. |
| Strings | KMP/Z, double hash, dynamic counted trie, Aho, suffix array/LCP, Manacher, minimum rotation | H simple hash/dynamic trie; Jet suffix and multi-pattern contracts; André alternatives deduplicated. |
| Sequences/DP | Compression, windows, monotonic stack/deque, LIS, classic DP, word-parallel subset sum, SOS, digit DP, D&C layer, XOR basis, intervals/histogram, cycles, CHT, dates | H efficient subset sum and classic implementations; Jet extra utility routines; André digit/D&C/XOR snippets completed into callable templates. |

## Non-equivalent specialist exclusions

These are explicit page-budget/scope decisions, **not claims that the techniques cannot appear at MIUP**, and not disguised duplicates. Consult the original notebooks or reserve sources if that specialist need arises.

| Input capability | Decision / remaining limitation |
|---|---|
| André `graph/LinkCutTree.h` | Online link/cut in changing forests is not replaced by fixed-tree HLD; splay/pointer complexity and page budget. |
| André `graph/DirectedMST.h` | Directed minimum arborescence is distinct from undirected Kruskal and Matrix-Tree counting; specialist complexity and page budget. |
| André `strings/SuffixTree.h` | Online Ukkonen tree is not equivalent to static suffix array/LCP; fixed capacity/alphabet and incomplete sentinel example require care. |
| André `geometry/Angle.h` | Specialised angle-order representation; basic cross/dot/atan2 remain, without claiming identical API. |
| André `geometry/linearTransformation.h` | Specialised similarity transform; ordinary Point operations remain. |
| André `number-theory/FastEratosthenes.h` | SPF is the canonical bounded sieve/factor interface; compressed huge-sieve memory/performance advantage is not preserved. |
| André `number-theory/ContinuedFractions.h` | Specialised rational approximation, distinct from real bisection; requires separate numerical/overflow contracts. |
| André `number-theory/FracBinarySearch.h` | Bounded rational search, distinct from integer/real search; supplied example/domain and overflow issues. |
| André `numerical/Polynomial.h` | Standalone polynomial mutation helpers not printed; Horner use explained alongside interpolation. |
| André `numerical/PolyRoots.h` | Supplied solver is not robust for constants/repeated roots/bounded-domain clipping; no blanket arbitrary-root claim. |
| André `numerical/FastFourierTransformMod.h` | Arbitrary-mod FFT reconstruction has a separate precision contract; one-prime NTT is not an equivalent replacement. |
| André `combinatorial/IntPerm.h` | Permutation rank/unrank omitted; not equivalent to cycle decomposition. |

Additional trade-offs: GoldenSearch becomes ternary search (more evaluations); Euler/RMQ LCA becomes binary lifting; batched modular inverses are not a second printed API; arbitrary-mod FFT convolution is not provided by the single-prime NTT. Dense vs sparse lazy remain separate because their domains differ.

## Prose and formula coverage

- **Preserved/condensed:** probability and expectation; quadratic equations, sums/progressions, exact arithmetic; gcd/Diophantine/modular identities; divisor/phi/Mobius formulas; inclusion–exclusion, stars-and-bars, Catalan, Lucas, derangements, Stirling/Bell/Eulerian/partition/Burnside/Cayley/Matrix–Tree references; graph degree/arborescence facts; bit operations and complexity reference.
- **Guide material removed from print:** H RecognitionGuide/Recognition appendix, RangeGuide, GraphGuide, SearchExample, EquationGuide, DPNotes/AppliedExamples narratives and WorkedExamples problem wrappers; Jet/André techniques checklist. Their algorithmic primitives remain where listed, while ClassicDP preserves generic executable routines from DPExamples.
- **Specialist prose not copied wholesale:** André Bernoulli/Euler–Maclaurin, permutation-cycle generating functions, hard-coded prime/estimate lists and extensive continuous-distribution/geometry formula pages. Useful selected formulas are kept; incorrect or ambiguous source claims were not propagated. See math/geometry reports for scope.
- **Inactive source is separate:** commented-out Mo, simplex, integration, Berlekamp–Massey, alternative flow engines, general-graph matching and other inherited KACTL files are listed as nonprinted source inventory in JSON. Their mere presence does not enlarge the claimed tested implementation set.

## Where the rationale and validation live

- [math.md](math.md): numerical, number-theoretic and combinatorial selection, constraints and oracle tests.
- [graphs.md](graphs.md): graph/tree/flow interfaces, specialist exclusions and tests.
- [geometry.md](geometry.md): geometry interface consolidation and validation.
- [ds-strings-various.md](ds-strings-various.md): DS/strings/DP choices, trade-offs and tests.

The strict target is **25 content pages + cover**, keeping the H colour/editorial theme. The final generated PDF and root validation report, rather than this inventory, establish the actual page count and final executed-test status. Original inputs/reserve material must not be mistaken for tested team-standard headers.
