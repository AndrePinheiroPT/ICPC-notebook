/**
 * Author: Team adaptation of Lucian Bicsi's KACTL LinearRecurrence
 * Summary: Find a far-away term of a known linear recurrence.
 * Description: s has the first d terms, c has d coefficients, d>0:
 *  s[t]=c[0]*s[t-1]+...+c[d-1]*s[t-d]. Pass k>=0 and mod>=1;
 *  signed values are normalized. Returns s[k] modulo mod (composite allowed).
 *  Reduces powers of x modulo the recurrence polynomial; combine multiplies
 *  those coefficient vectors. $O(d^2\log(k+1))$ time, O(d) extra memory.
 * Usage: assert(linearRec({0,1},{1,1},10,1000000007) == 55);
 */
#pragma once

ll linearRec(vec<ll> s, vec<ll> c, unsigned long long k, ll mod) {
    int d = sz(c);
    assert(d > 0 && sz(s) == d && mod >= 1);
    for (auto* v : {&s,&c}) for (ll& x : *v) {
        x %= mod; if (x < 0) x += mod;
    }
    auto combine = [&](const vec<ll>& a, const vec<ll>& b) {
        vec<ll> r(2*d-1);
        rep(i,0,d) rep(j,0,d)
            r[i+j] = ll((r[i+j]+(__int128)a[i]*b[j])%mod);
        for (int i = 2*d-2; i >= d; --i) rep(j,0,d)
            r[i-1-j] = ll((r[i-1-j]+(__int128)r[i]*c[j])%mod);
        r.resize(d); return r;
    };
    vec<ll> a(d), x(d); a[0] = 1%mod;
    if (d == 1) x[0] = c[0]; else x[1] = 1%mod;
    while (k) {
        if (k&1) a = combine(a,x);
        k >>= 1; if (k) x = combine(x,x);
    }
    ll ans = 0;
    rep(i,0,d) ans = ll((ans+(__int128)a[i]*s[i])%mod);
    return ans;
}
