/**
 * Author: Team adaptation of KACTL Determinant
 * Summary: Determinant of a real square matrix.
 * Description: Copies a and returns det(a); det(empty)=1. Row swaps flip
 *  the sign. Partial pivoting; pivots with absolute value <=eps count as zero.
 *  Finite arithmetic required. $O(n^3)$ time, $O(n^2)$ copied memory.
 * Usage: assert(abs(determinant({{2,1},{1,3}})-5) < 1e-9L);
 */
#pragma once

long double determinant(vec<vec<long double>> a,
        long double eps = 1e-12L) {
    int n = sz(a); long double ans = 1;
    assert(eps > 0);
    for (auto& row : a) assert(sz(row) == n);
    rep(i,0,n) {
        int p = i;
        rep(j,i+1,n) if (abs(a[j][i]) > abs(a[p][i])) p = j;
        if (abs(a[p][i]) <= eps) return 0;
        if (p != i) { swap(a[p],a[i]); ans = -ans; }
        ans *= a[i][i];
        rep(j,i+1,n) {
            long double f = a[j][i]/a[i][i];
            rep(k,i+1,n) a[j][k] -= f*a[i][k];
        }
    }
    return ans;
}
