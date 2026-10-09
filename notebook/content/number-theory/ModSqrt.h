/**
 * Author: Team adaptation of Simon Lindholm's KACTL ModSqrt
 * Summary: Solve $x^2=a$ modulo a prime with Tonelli--Shanks.
 * Description: Signed a and PRIME p>=2 (up to LLONG\_MAX). Returns optional
 *  root x in [0,p), nullopt if no root; the other root is (p-x)\%p.
 *  Handles p=2 and a=0. Products use 128 bits. Main loop $O(\log^2 p)$
 *  modular operations, plus the search for a quadratic non-residue;
 *  no small worst-case bound claimed for that search. O(1) memory.
 * Usage: auto x = modSqrt(10,13); // roots 6 and 7
 */
#pragma once
#include "../numerical/ModPower.h"

optional<ll> modSqrt(ll a, ll p) {
    assert(p >= 2); a %= p; if (a < 0) a += p;
    if (p == 2 || a == 0) return a;
    if (modPow(a,(p-1)/2,p) != 1) return nullopt;
    if (p%4 == 3) return modPow(a,p/4+1,p);
    ll d = p-1, z = 2; int r = 0;
    while (d%2 == 0) { d /= 2; ++r; }
    while (modPow(z,(p-1)/2,p) != p-1) ++z;
    ll x = modPow(a,(d+1)/2,p), t = modPow(a,d,p);
    ll c = modPow(z,d,p);
    while (t != 1) {
        ll u = t; int i = 0;
        while (u != 1) { u = modMul(u,u,p); ++i; }
        ll b = modPow(c,1ULL<<(r-i-1),p);
        x = modMul(x,b,p); c = modMul(b,b,p);
        t = modMul(t,c,p); r = i;
    }
    return x;
}
