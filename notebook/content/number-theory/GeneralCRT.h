/**
 * Author: jet notebook
 * Summary: Combine congruences with possibly non-coprime moduli.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Combine two repeating schedules: x has remainder a
 *  modulo m and remainder b modulo n. m,n>0; any signed a,b.
 *  \item crtMerge(a,m,b,n) returns (r,M): all solutions r+k*M,
 *  0<=r<M, M=lcm(m,n). (-1,0): incompatible; (0,-1): M
 *  exceeds ll. Moduli need not be coprime.
 *  \item $O(\log(\max(m,n)))$ time/space.
 *  \end{itemize}
 * Usage: auto [r,m] = crtMerge(2,6,5,9);
 *  // (14,18); check m>0 before using r
 */
#pragma once
#include "ExtGCD.h"

pair<ll,ll> crtMerge(ll a, ll m, ll b, ll n) {
	assert(m > 0 && n > 0);
	a %= m; if (a < 0) a += m;
	b %= n; if (b < 0) b += n;
	ll x, y, g = extGcd(m,n,x,y), d = b-a;
	if (d%g) return {-1,0};
	if (m/g > LLONG_MAX/n) return {0,-1};
	ll M = (m/g)*n;
	ll k = ll((__int128)(d/g)*x % (n/g));
	ll r = ll(((__int128)m*k+a)%M);
	return {r < 0 ? r+M : r,M};
}
