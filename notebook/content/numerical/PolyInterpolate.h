/**
 * Author: Team adaptation of KACTL polynomial interpolation
 * Summary: Recover polynomial coefficients from distinct sample points.
 * Description: x,y have n entries, all x distinct. Returns a of length n
 *  such that $p(t)=\sum_i a_i t^i$ and p(x[i])=y[i]; empty input gives empty output.
 *  Uses divided differences then converts to ordinary coefficients.
 *  Floating point: clustered/large x or high degree may be ill-conditioned.
 *  $O(n^2)$ time, O(n) extra memory. Evaluate with Horner from highest degree.
 * Usage: auto a = interpolate({0,1,2},{1,4,9}); // {1,2,1}
 */
#pragma once

vec<long double> interpolate(const vec<long double>& x,
        vec<long double> y) {
    int n = sz(x); assert(sz(y) == n);
    rep(k,0,n) rep(i,k+1,n) {
        assert(x[i] != x[k]);
        y[i] = (y[i]-y[k])/(x[i]-x[k]);
    }
    vec<long double> a(n), basis(n+1); basis[0] = 1;
    rep(k,0,n) {
        rep(i,0,k+1) a[i] += y[k]*basis[i];
        for (int i = k+1; i > 0; --i)
            basis[i] = basis[i-1]-x[k]*basis[i];
        basis[0] *= -x[k];
    }
    return a;
}
