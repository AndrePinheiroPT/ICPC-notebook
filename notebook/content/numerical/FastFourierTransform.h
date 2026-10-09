/**
 * Author: Team notebook; iterative Cooley--Tukey FFT
 * Summary: Approximate convolution of real coefficient/frequency arrays.
 * Description: FFT::conv(a,b)[k]=sum a[i]*b[k-i]; length |a|+|b|-1,
 *  empty if either input is empty. No modulus; accepts negative/real entries.
 *  Round only when the accumulated error is comfortably below 0.5;
 *  large coefficients/lengths need an independent error bound or exact NTT.
 *  fft changes a power-of-two complex array in place; inverse=true undoes it.
 *  Padded size N: O(N log N) time, O(N) memory.
 * Usage: auto c = FFT::conv({1,2},{3,4}); // approximately {3,10,8}
 */
#pragma once

namespace FFT {
using C = complex<long double>;
void fft(vec<C>& a, bool inverse = false) {
    int n = sz(a); assert(n > 0 && !(n&(n-1)));
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n>>1;
        for (; j&bit; bit >>= 1) j ^= bit;
        j ^= bit; if (i < j) swap(a[i],a[j]);
    }
    for (int len = 2; len <= n; ) {
        C root = polar(1.0L, (inverse?-2:2)*acosl(-1.0L)/len);
        for (int i = 0; i < n; i += len) {
            C w = 1;
            rep(j,0,len/2) {
                C u = a[i+j], v = a[i+j+len/2]*w;
                a[i+j] = u+v; a[i+j+len/2] = u-v; w *= root;
            }
        }
        if (len == n) break;
        len *= 2;
    }
    if (inverse) for (C& x : a) x /= n;
}
vec<long double> conv(const vec<long double>& a,
        const vec<long double>& b) {
    if (a.empty() || b.empty()) return {};
    int s = sz(a)+sz(b)-1, n = 1;
    assert(s > 0 && s <= (1<<29));
    while (n < s) n *= 2;
    vec<C> x(n), y(n);
    rep(i,0,sz(a)) x[i] = a[i];
    rep(i,0,sz(b)) y[i] = b[i];
    fft(x); fft(y);
    rep(i,0,n) x[i] *= y[i];
    fft(x,true); vec<long double> c(s);
    rep(i,0,s) c[i] = x[i].real();
    return c;
}
}
