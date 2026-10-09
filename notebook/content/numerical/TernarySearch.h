/**
 * Author: Team notebook
 * Summary: Approximate the minimum of a unimodal real function.
 * Description: Pass finite l<=r, function f and positive iteration count.
 *  On [l,r], f must decrease then increase (either side may be empty).
 *  Returns an approximate minimizer x, not f(x). Flat minimum is allowed;
 *  other flat regions can invalidate the search. Negate f to maximize.
 *  O(iterations) evaluations, O(1) memory. Use binary search for monotone predicates.
 * Usage: auto x = ternaryMin(0,10,[](long double x) { return (x-3)*(x-3); });
 */
#pragma once

template<class F>
long double ternaryMin(long double l, long double r,
        F f, int iterations = 100) {
    assert(isfinite(l) && isfinite(r) && l <= r && iterations > 0);
    rep(it,0,iterations) {
        long double a = l+(r-l)/3, b = r-(r-l)/3;
        if (f(a) <= f(b)) r = b;
        else l = a;
    }
    return l+(r-l)/2;
}
