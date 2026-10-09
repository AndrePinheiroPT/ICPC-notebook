/**
 * Author: Team notebook; linear sieve
 * Summary: Euler phi and Mobius mu for all integers through N.
 * Description: phi[n] counts integers 1..n coprime to n; mu[n] is zero
 *  for a squared prime factor, otherwise $(-1)^k$ for k distinct primes.
 *  numberFunctions(N) returns the pair (phi,mu), both N+1 entries; index 0 unused.
 *  phi[1]=mu[1]=1. N>=1. lp stores least prime factors internally.
 *  O(N) time and memory. See divisor-sum identities in the chapter.
 * Usage: auto [phi,mu] = numberFunctions(12); // phi[12]=4, mu[12]=0
 */
#pragma once

pair<vec<int>,vec<int>> numberFunctions(int n) {
    assert(n >= 1);
    vec<int> phi(n+1), mu(n+1), lp(n+1), primes;
    phi[1] = mu[1] = 1;
    rep(i,2,n+1) {
        if (!lp[i]) { lp[i]=i; phi[i]=i-1; mu[i]=-1; primes.pb(i); }
        for (int p : primes) {
            if (p > n/i) break;
            int v = i*p; lp[v] = p;
            if (i%p == 0) { phi[v] = phi[i]*p; mu[v] = 0; break; }
            phi[v] = phi[i]*(p-1); mu[v] = -mu[i];
        }
    }
    return {phi,mu};
}
