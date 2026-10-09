/**
 * Author: Team adaptation of Bjorn Martinsson's KACTL ModLog
 * Summary: Discrete logarithm: smallest positive exponent $a^x=b$ modulo m.
 * Description: Signed a,b are normalized; $1\le m\le10^{12}$. Returns x>=1,
 *  or -1 if impossible. Works even when gcd(a,m)>1; modLog(a,1,m)
 *  gives the multiplicative order if gcd(a,m)=1. For nonnegative x,
 *  check b==1 modulo m first and return 0 yourself.
 *  Baby-step giant-step: O(sqrt(m)) expected time and memory;
 *  the modulus bound limits the table to about one million entries.
 * Usage: assert(modLog(2,8,13) == 3);
 */
#pragma once
#include "../numerical/ModPower.h"

ll modLog(ll a, ll b, ll m) {
    assert(1 <= m && m <= 1000000000000LL);
    a %= m; if (a < 0) a += m;
    b %= m; if (b < 0) b += m;
    ll n = ll(sqrtl(m))+1, e = 1%m, f = e, j = 1;
    unordered_map<ll,ll> baby;
    while (j <= n) {
        e = f = modMul(e,a,m);
        if (e == b) return j;
        baby[modMul(e,b,m)] = j++;
    }
    if (gcd(m,e) == gcd(m,b))
        for (ll i = 2; i <= n+1; ++i) {
            e = modMul(e,f,m);
            auto it = baby.find(e);
            if (it != baby.end()) return n*i-it->second;
        }
    return -1;
}
