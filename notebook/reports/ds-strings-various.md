# Data structures, strings and sequences/DP: selection and verification

## Selection method

Inspected the active `chapter.tex` imports in all three input archives, then read the implementations and variants. The team chapters contain **40 canonical headers**: 16 data structures, 8 strings, 16 sequences/DP. Inactive legacy variants are not treated as extra printed features. This is an encyclopedia selection; the earlier TST-oriented cuts have not been repeated.

All selected headers use `vec<T>` instead of `vi`/`vector`, `pb`, and `rep` for ordinary increasing loops. Special loop shapes remain explicit. Public indices are 0-based, ranges `[l,r)`, prefix arrays `n+1`. DSU's internal negative-size representative entries and Fenwick's internal 1-based bookkeeping are documented exceptions, not tree-parent conventions. Documentation is principally English, with compact input/output contracts, complexities, helper explanations and tiny canonical usages. The existing final bit/complexity reference retains clear PT-PT prose.

## Canonical implementations and important interface changes

| Technique | Chosen source and decision |
|---|---|
| Prefix/difference arrays 1D and 2D | Gonçalo's explicit safe interfaces retained. Empty ranges and non-mutating finalisation documented. |
| DSU | Common KACTL implementation from all notebooks; preserved `UF(n)`, `find`, `join`, `sameSet`, `size`, so graph consumers remain compatible. |
| Rollback DSU / dynamic connectivity | Restored Jet's `RollbackUF` and segment tree over edge lifetimes. Preserved `time/rollback` and `DynamicConnectivity::add(l,r,u,v), run(callback)`. Lifetimes and repeated-edge copies are explicit. |
| Fenwick 1D | Gonçalo's version retains prefix selection `lowerBound`; query/add interfaces remain compatible with Fenwick2D. Search requires nonnegative values, sentinel -1 for nonpositive target and n when absent. |
| Fenwick 2D | Jet's offline coordinate registration, supports arbitrary rectangle query boundaries. Explicit `Fenwick.h` dependency. |
| Iterative segment tree | Common KACTL ordered-accumulator version, now `SegTree` with `ll` values, `LLONG_MIN` max identity, and explicit associative `merge`. Keeps efficient general monoid adaptation. `query` preserves order for a future noncommutative merge. |
| Dense lazy tree | Combined Gonçalo/Jet's range-add/sum use with André's range-assignment/max functionality in ONE implementation: `add`, `set`, `query` (sum), `queryMax`. Uses explicit assignment flag instead of reserving an integer value. All intermediates, including cumulative lazy tags, must fit T even when the final individual values fit. |
| Sparse lazy tree | André's distinct online-large-domain use retained as `SparseLazySegTree`: ll index domain `[0,n)`, initially zero, range add/set and max. Arena-owned indices avoid global `Node` collisions, raw allocation ownership and allocator dependencies. Query may allocate; documented memory bound. Pending tags use `__int128`, avoiding overflow when large additions cancel against old values. |
| Sparse table | Gonçalo's simple minimum `RMQ<T>` retained. `jmp[k][i]`, nonempty `[l,r)`; supports pairs for first argmin. Explains idempotence and why overlapping-block query is unsuitable for sum. |
| Sqrt decomposition | Jet's removal/insertion update retained, with team-agreed `countLE(l,r,x)` and `upper_bound`. Scalar check uses only `<`; generic ordered values supported. |
| Order statistics | Jet's `OST<T>` PBDS alias, duplicate recipe `(value,unique_id)`, end-check for kth documented. |
| Hash map | André's integer hash-map need retained with a short salted splitmix hash and standard `unordered_map` API; no global mutable example map or PBDS-only capacity constructor. Exact key equality preserved. |
| Implicit treap | André's positional split/merge approach adapted into `ImplicitTreap`, with private-by-convention arena helpers and no global Node/split names. Public insert, erase, reverse, sum, move and materialise. `move(l,r,k)` measures k in the sequence AFTER removing the block. Deleted nodes remain allocated, explicitly documented. |
| KMP / Z | Gonçalo/Jet generic sequence routines; KMP includes overlaps and empty-pattern boundary matches. Border/period contracts explained. |
| Double hash | Gonçalo H's simple two fixed distinct bases, prefix powers local to object, key includes length, unsigned string bytes, O(1) substring and concatenation. Joint coordinate compression required for signed/64-bit sequences. Deliberate collision risk remains explicit; no claim of deterministic equality. |
| Trie | Gonçalo's dynamic vector of nodes and duplicate counts. Keeps empty-word handling, `count`, `countPrefix`, `contains`, `walk` and exposed edges for adaptations. |
| Aho--Corasick | Jet's build-once automaton: counts and reporting via output links, duplicate IDs and empty patterns. Does not copy all output lists onto every node. |
| Suffix array + LCP | Jet's O(n log n) rank-doubling plus linear LCP; byte ordering and n/n/n-1 lengths are explicit. Arbitrary-suffix LCP reuses canonical RMQ. |
| Manacher / minimum rotation | Restored Jet's generic sequence routines with precise radii, empty-input and tie conventions. Replaces André variants of the same techniques. |
| Coordinate compression / windows / monotonic stack and deque | Gonçalo's usable standalone routines retained, shortened to contracts rather than worked problem stories. Conditions on nonnegative sums and strict/tied boundaries remain. |
| LIS | Gonçalo's explicit binary search version; returns indices and supports nondecreasing variant. |
| Basic DP | Actual `DPExamples.h` code becomes `ClassicDP.h`: Kadane linear/circular, 0/1 knapsack, minimum coins, unordered coin combinations, LCS length, grid path count. Long guide and worked narratives are removed from print, but these algorithmic routines remain. |
| Subset sum / SOS | Gonçalo's word-parallel bitset shift/OR retained (Jet's scalar O(nN) regression is NOT reintroduced); general associative/commutative SOS supports submasks and supermasks. |
| XOR basis | André's idea completed into a compiled 64-bit unsigned `XorBasis` with insert/independence, membership, maximum and rank; source snippet lacked a terminating semicolon and only provided insertion/merge. |
| Digit DP | André's unfinished hooks/snippet replaced by an executable canonical digit-sum remainder DP, including arbitrary-length bounds, modulo counting, zero/leading-zero contract and how to adapt the state. |
| Divide-and-conquer DP | André/KACTL placeholder sketch replaced by `partitionLayer(prev,cost,INF)` with `[l,r)` partition costs. Leftmost optimal split MUST be monotone; finite/intermediate value sentinel bounds are checked with `__int128`. |
| Interval cover / histogram / Floyd cycle | Jet's implementations restored/retained. Return IDs/boundaries and weighted widths remain, not just objective values. |
| Dynamic convex hull trick | Jet/KACTL `LineHull` selected over duplicate André `LineContainer`; breakpoints/differences/evaluation now use `__int128`, with ll result assertion. Supports arbitrary slope order and duplicates. No dependency on floorDiv remains. |
| Gregorian dates | Jet's date-to-day conversion retained, with valid date and year-domain contract; uses shared `IntDivision.h`. |

## Deliberate exclusions / reservations

- **André's Ukkonen suffix tree:** not printed. Distinct online suffix-tree construction is specialised, substantially harder to adapt, uses a fixed ~200k-node/26-letter arena, and the example LCS needs an explicit alphabet increase for two sentinels. The printed suffix array/LCP handles the team's usual static suffix/substring queries; this is not a claim that it is a full online suffix-tree replacement. Source retained in reserve.
- **Mo queries:** commented out in André's active chapter, absent from Gonçalo/Jet active selections; not newly introduced merely to expand the library. Reserved source still available.
- **Duplicate recursive/iterative segment-tree and Fenwick variants, generic operation RMQ, duplicate hashing/trie/Manacher/rotation implementations:** one canonical interface each. Dense and sparse lazy trees remain separate because online huge coordinate domains are a different need.
- **André's binary-search and ternary-search snippets:** numerical chapter owns the canonical versions; no second version in Various.
- **André's data-structures/Matrix:** numerical chapter owns canonical matrices; no duplicate header in this chapter.
- **Gonçalo's long RangeGuide, DPNotes, AppliedExamples, WorkedExamples and problem-solving narrative:** removed from the printed encyclopedia. `ClassicDP` retains actual general DP routines. The more problem-specific equal-key-pair, circular-character-window, occupied-time and dictionary-segmentation wrappers are left in reserve rather than printing a worked-problem collection. The underlying hashing/sorting, windows, intervals, trie and DP tools remain.
- **André's micro-optimisation pragmas, hardware-specific SIMD/denormal modes, allocator tricks and converting crashes into Wrong Answer:** not adopted into the team reference; they do not improve the canonical algorithm interfaces and can obscure diagnosis or portability.

## Verification performed

Tests are deterministic, finite and directly include every one of these 40 printed headers across the three test binaries. Each binary combines the headers in its group to detect collisions, and instantiates/calls their public APIs. Full all-chapter and isolated-header verification is additionally coordinated by the main task.

| Test file | Release checks passed | ASan + UBSan checks passed |
|---|---:|---:|
| `tests/team/ds.cpp` | 760119 | 760119 |
| `tests/team/strings.cpp` | 599368 | 599368 |
| `tests/team/various.cpp` | 376912 | 376912 |
| **Total per mode** | **1736399** | **1736399** |

- **DS:** direct-array oracles for 1D sums, min/max trees, mixed add/set lazy operations, sqrt threshold counts, 2D rectangles, prefix selection; brute connectivity for UF/rollback/time lifetimes; treap versus vector under insert/erase/reverse/move/sum; OST and map against sorted/map references. Empty arrays/ranges, repeated runs and a sparse domain ending at `LLONG_MAX` and cancelling lazy additions whose net tag exceeds ll are covered.
- **Strings:** exhaustive small substring/border/palindrome checks, brute suffix sorting/LCP and minimum rotation, direct substring polynomial oracle using `__int128`, concatenation, byte values 0..255, integer symbols up to modulus-1, leading zero symbols, trie duplicate/empty prefixes and Aho duplicate/empty patterns. Also 100000 equal characters for suffix/KMP/Manacher/rotation behaviour.
- **Various:** subset enumeration for LIS, knapsack, XOR span and interval-cover optimum; direct windows/histogram; every submask/supermask sum and max for small SOS; digit enumeration and a 20-digit-bound modular count; O(n²) DP layers vs D&C on proven monotone squared nonnegative segment sums; exhaustive circular Kadane; BFS minimum coins, recursive unordered coin count, subsequence enumeration for LCS, recursive grid paths; visited-state Floyd oracle; full scan of inserted lines including ll-extreme coefficients; Gregorian calendar iteration 1970--2030 and 400-year/negative-year identities.

Commands from notebook root:

```sh
g++ -std=c++17 -O2 -Wall -Wextra tests/team/ds.cpp -o /tmp/team-ds
/tmp/team-ds
# Repeat for strings.cpp and various.cpp.
g++ -std=c++17 -O1 -g -fsanitize=address,undefined \
  -fno-omit-frame-pointer -D_GLIBCXX_ASSERTIONS tests/team/ds.cpp -o /tmp/team-ds-san
ASAN_OPTIONS=detect_leaks=0 /tmp/team-ds-san
```

ASan/UBSan passed. LeakSanitizer cannot run in this host's ptrace environment (`LeakSanitizer does not work under ptrace`), so leak detection was disabled explicitly; no claim of a successful LSan run. These implementations own storage through standard containers; erased treap nodes intentionally remain allocated for the object's lifetime as documented.

## Literal printed Usage validation

`tests/usage_examples.py` re-extracts the actual `Usage:` block from each of all **40 active headers** in these chapters, compiles the extracted text unchanged, and executes it with **61 explicit postcondition checks**. Where a snippet references pre-existing inputs (`points`, `text`, `s`, `n`, `prefix`, `next`, or an already-built dynamic-connectivity object), the test supplies only a minimal fixture before the literal snippet. No implementation/API rewrite is performed in the test. Coverage is checked against current chapter imports, so adding/removing a printed header cannot silently bypass this audit.

All 40 snippets use current interfaces and their claimed outputs are correct; no printed documentation changes were necessary. The suffix-LCP commented usage is exercised with a separate postcondition using exactly the documented rank-range formula. Aho's literal output callback is captured and checked. Executable wrappers are temporary files and are removed after testing.

```sh
python3 tests/usage_examples.py
python3 tests/usage_examples.py --sanitize
```

Both release and ASan/UBSan runs pass: `literal Usage: 40 headers, 61 checks passed`.
