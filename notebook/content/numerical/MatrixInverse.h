/**
 * Author: Team notebook; Gauss--Jordan elimination
 * Summary: Invert a real square matrix, with explicit failure.
 * Description: matrixInverse(a,eps) copies a and returns optional inverse;
 *  nullopt if a pivot is <=eps in absolute value. Uses partial pivoting.
 *  eps is absolute: scale inputs, and expect floating-point error.
 *  $O(n^3)$ time and $O(n^2)$ memory. Empty matrix has an empty inverse.
 * Usage: auto inv = matrixInverse({{2,1},{1,3}}); // test inv before *inv
 */
#pragma once

optional<vec<vec<long double>>> matrixInverse(
        vec<vec<long double>> a, long double eps = 1e-12L) {
    int n = sz(a); assert(eps > 0);
    vec<vec<long double>> b(n,vec<long double>(n));
    rep(i,0,n) { assert(sz(a[i]) == n); b[i][i] = 1; }
    rep(i,0,n) {
        int p = i;
        rep(j,i+1,n) if (abs(a[j][i]) > abs(a[p][i])) p = j;
        if (abs(a[p][i]) <= eps) return nullopt;
        swap(a[p],a[i]); swap(b[p],b[i]);
        long double v = a[i][i];
        rep(k,0,n) { a[i][k] /= v; b[i][k] /= v; }
        rep(j,0,n) if (j != i) {
            long double f = a[j][i];
            rep(k,0,n) { a[j][k] -= f*a[i][k]; b[j][k] -= f*b[i][k]; }
        }
    }
    return b;
}
