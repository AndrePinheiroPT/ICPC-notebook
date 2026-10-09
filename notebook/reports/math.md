# Mathematics, numerical methods, number theory and combinatorics

## Printed selection (34 headers)

Selection was based on the active imports of all three inputs, not the number of `.h` files in an archive. The H sources also contain historical unprinted KACTL files; those do not automatically become printed material.

| Capability | Selected source / integration |
|---|---|
| Integer division/alignment, 128-bit I/O | Jet/H versions retained; English contracts, correct negative operands, no lost validated parser. |
| Basic probability | H implementation retained (binomial distribution, at least one, geometric); compact English usage, probability/expectation reference retained. |
| Binary search | **Current H** `[l,r)` + `optional<ll>` interface retained, with 128-bit midpoint. Empty/no-answer is `nullopt`; conceptual excluded sentinels are not evaluated. This avoids reintroducing the older sentinel convention into half-open team interfaces. |
| Bisection | H real-valued predicate search retained. |
| Unimodal search | Straightforward `ternaryMin` introduced in Numerical, replacing GoldenSearch in print; same asymptotic search cost, more function evaluations. Golden variants stay in reserve for expensive evaluations. |
| Modular arithmetic | H generic `modMul`/`modPow` consolidated as the sole printed modular product/power implementation; 128-bit intermediates, explicit types/contracts and assertions. |
| Matrices | H vector-based modular `matMul`/`matPow`, restoring printed availability. Validates shape/residues even when exponent is zero. Column-state orientation documented. |
| Real / modular / binary systems | H real Gauss and normalized Gauss–Jordan prime-mod Gauss; Jet `GaussBinary` restored. All return rank, -1 inconsistency, and one solution with free variables zero. |
| Determinants | André/KACTL capability retained: real determinant with partial pivoting, composite-modulus determinant with Euclidean row elimination and 128-bit products. Distinct names prevent overload/global-mod conflicts. |
| Matrix inverse | André capability retained with a simpler Gauss–Jordan `optional` API and a clear singularity contract. |
| Polynomial interpolation | André divided-difference approach, using `long double`, explicit sizes, empty case and conditioning caveat; returns increasing-degree coefficients. |
| Linear recurrence | André/KACTL polynomial reduction approach, rewritten with coefficient vectors of size d and explicit modulus; unsigned 64-bit index (no overflowing `++k`), normalized signed coefficients, 128-bit products. |
| Exact convolution | Jet/H NTT at 998244353, restored in print; independent namespace, empty handling, power-of-two and length limits. |
| Real convolution | André FFT capability kept via a straightforward iterative complex `long double` FFT. Avoids its pointer-reinterpretation shortcut/global cached roots; explicit approximate result and no unsupported unconditional integer-rounding guarantee. Same O(N log N). |
| Extended Euclid, inverse, CRT | Jet/H versions retained, preserving inverse optional and CRT distinct incompatible/overflow status; generalized CRT covers non-coprime moduli. |
| Prime sieve/factorization/divisors | SPF and trial division retained; deterministic 64-bit Miller–Rabin and randomized Pollard–Rho/Brent restored. All factorization outputs are sorted prime factors with repeats; divisors consumes that representation. |
| Discrete logarithm | André/KACTL non-coprime-capable BSGS retained, safe multiplication, normalized inputs, smallest **positive** exponent and -1 failure documented. Explicit modulus <=10^12 bounds the sqrt table to about one million entries. |
| Modular square root | André/KACTL Tonelli–Shanks adapted to return optional instead of asserting existence; p=2/zero handled, safe products and prime-only requirement. |
| Floor/remainder sums | André capability rewritten as Euclidean reduction, negative slope/offset supported, signed 128-bit result. Explicit <=10^9 parameter bounds give safe intermediates. |
| Totient/Mobius | André phi sieve replaced by a dynamic O(N) sieve also providing Mobius values; no fixed five-million global array. Outputs have N+1 entries. |
| Exact/modular combinations, permutation cycles | H versions retained; `fact`, `invfact`, `choose`, `cycleLengths`, explicit prime/n<p and exact-overflow requirements. |

All printed owned code uses `vec<T>` rather than `vi`/`vector`, and `pb`; simple loops use `rep` where natural, while loops with differing traversal requirements remain explicit. All array results and indices are zero-based. No fixed global `mod`, `Poly` alias collision or global `sqrt` overload was introduced.

## André-specific decisions and reserve material

The original archive remains the provenance source; unprinted headers in these directories remain reserve material and **are not claimed to comply with the team API or to pass the active suite**.

- `ModularArithmetic`, `ModInverse`, `ModPow`, `ModMulLL`, `euclid`, `CRT`, MillerRabin/Factor: consolidated into the corresponding selected helpers above. An extra operator-heavy modular class or a second inverse/CRT implementation would add duplicate interfaces.
- `FastEratosthenes`: reserve. SPF gives the directly usable bounded-prime/factor interface; bit-compressed huge sieves serve a different performance corner and carry more code/complexity. No claim of equal memory usage.
- `phiFunction`: replaced by `TotientMobius`, preserving totient queries with dynamic limits and adding the directly related Mobius sieve.
- `ContinuedFractions`, `FracBinarySearch`: reserve. Useful specialised rational searches, but not selected for this MIUP-sized union; the raw fractional search's example is inconsistent with its [0,1] domain, and overflow/endpoint contracts need separate care. Ordinary integer/real search remains printed; this is a scope exclusion, not equivalent functionality.
- `Polynomial`: reserve. Horner evaluation is explained next to interpolation. Mutation helpers for polynomial division/differentiation are not a separate printed API.
- `PolyRoots`: reserve. Supplied code does not robustly implement arbitrary bounded real-root enumeration (notably repeated roots, constants, and clipping linear roots). It was not copied as a supposedly general solver. Bisection/ternary have narrower, explicit contracts.
- `PolyInterpolate`, `LinearRecurrence`: included as above.
- `SolveLinear`, `SolveLinear2`, `SolveLinearBinary`: consolidated into the three Gauss interfaces; no second real solver/different output representation.
- `Determinant`, `IntDeterminant`, `MatrixInverse`: included as above. Composite determinant retained rather than incorrectly replaced by prime-field Gaussian elimination.
- `FastFourierTransform`: included as above. `NumberTheoreticTransform`: consolidated with Jet NTT. `FastFourierTransformMod`: reserve (arbitrary-mod convolution is specialised; floating reconstruction brings a separate precision contract). No implication that one-prime NTT covers arbitrary moduli.
- `IntPerm`, `multinomial`: reserve implementation. Multinomial counting is retained as a formula using exact/modular binomial primitives. Ranking/unranking permutations is specialised and not replaced by cycle decomposition.
- André's commented-out Berlekamp–Massey, simplex, hill climbing, integration, adaptive integration, modular matrix inverse, tridiagonal solver and subset transforms were **not active input content**. Remain reserve; no automatic promotion based only on file existence.
- Added/retained useful formula references: Lucas; derangements; Burnside/necklaces; integer partitions; unsigned Stirling I/II, Bell and Eulerian numbers; Cayley and Matrix–Tree; Catalan; Pythagorean triples; Mobius inversion; variance, hypergeometric, Poisson and common continuous distributions.
- Excluded specialised/misleading André formula blocks from print: Bernoulli/Euler–Maclaurin, permutation cycle exponential generating function, arbitrary hard-coded prime list/divisor estimates, and its incorrect/overbroad normal-distribution claims. Recurrence characteristic roots and essential trigonometry retained compactly. Triangle/quadrilateral geometry identities are delegated to the geometry chapter decision rather than copied twice.
- Long TST problem-guide material (`SearchExample`, `EquationGuide`) is no longer imported. A short system-assembly paragraph and canonical per-header usage replace it.

## Validation performed

Three committed runners under `tests/team/` include the actual selected headers and the shared team prelude (no private `vi` alias or hidden helper implementation):

| Runner | Evidence per mode |
|---|---:|
| `math-core.cpp` | 235,261 counted comparisons; additional assertions for individual edge cases |
| `number-core.cpp` | 1,124,094 counted comparisons; additional assertions for 64-bit and failure cases |
| `math-kactl.cpp` | 687,812 exhaustive comparisons adapted from supplied KACTL recurrence, determinant and binary-system stress tests |

Modes: C++17 `-O2 -Wall -Wextra`; and `-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -D_GLIBCXX_ASSERTIONS`. Sanitized runs use `ASAN_OPTIONS=detect_leaks=0`: LeakSanitizer cannot run under this environment's ptrace supervision; address, undefined-behaviour and STL assertion checks stay enabled. Tests finish well within 30 seconds each.

Oracles include direct DP recurrences, exhaustive assignment counts/ranks, all small permutation determinant terms, identity checks for inverse/solutions, schoolbook convolution, brute discrete-log powers, brute floor/remainder sums, primality sieve, divisor enumeration, factor-product reconstruction, and exhaustive bounded CRT/inverse checks. Cases include empty inputs, negative coefficients, no/unique/multiple solutions, composite moduli, LLONG endpoint binary searches, ULLONG_MAX products, signed 128-bit I/O boundaries, malformed input, deterministic probability p=0/1, repeated/zero factors and large 64-bit semiprimes.

Typical local command, from the notebook root:

```sh
g++ -std=c++17 -O2 tests/team/math-core.cpp -o /tmp/math-core
/tmp/math-core
```

Replace the runner name for the other two. The root integration runner additionally handles isolated headers, all-header integration, usage compilation, PDF generation and layout review; these are not claimed as completed by this report alone.
