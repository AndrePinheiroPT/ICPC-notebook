/**
 * Author: Team adaptation of KACTL IntDeterminant
 * Summary: Determinant modulo any positive modulus, including composite.
 * Description: Copies square matrix a, normalizes signed entries, returns
 *  det(a) in [0,mod). Euclidean row reductions avoid modular division;
 *  useful for Matrix--Tree counts even when mod is not prime.
 *  $O(n^3\log mod)$ time upper bound, $O(n^2)$ copied memory.
 * Usage: assert(detMod({{2,1},{1,3}},6) == 5);
 */
#pragma once

ll detMod(vec<vec<ll>> a, ll mod) {
    assert(mod > 0); int n = sz(a); ll ans = 1%mod;
    for (auto& row : a) {
        assert(sz(row) == n);
        for (ll& x : row) { x %= mod; if (x < 0) x += mod; }
    }
    rep(i,0,n) {
        rep(j,i+1,n) while (a[j][i]) {
            ll q = a[i][i]/a[j][i];
            rep(k,i,n) {
                a[i][k] = ll((a[i][k]-(__int128)q*a[j][k])%mod);
                if (a[i][k] < 0) a[i][k] += mod;
            }
            swap(a[i],a[j]); if (ans) ans = mod-ans;
        }
        ans = ll((__int128)ans*a[i][i]%mod);
    }
    return ans;
}
