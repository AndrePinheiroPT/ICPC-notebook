/**
 * Author: Team notebook; Euclidean floor-sum reduction
 * Summary: Sum floors/remainders of an arithmetic progression without iterating n times.
 * Description: floorSum(n,m,a,b)=sum floor((a*i+b)/m), 0<=i<n.
 *  modSum uses mathematical nonnegative remainder instead of floor.
 *  n>=0, m>0; all four parameters $\le10^9$ in absolute value.
 *  Signed a,b allowed. Returns signed \texttt{\_\_int128}; print using Int128.h.
 *  O(log m) time, O(1) memory. Cast before products outside this function too.
 * Usage: assert(floorSum(4,3,2,1) == 4); // 0+1+1+2
 */
#pragma once

__int128 floorSum(ll n, ll m, ll a, ll b) {
    assert(0 <= n && n <= 1000000000 && 0 < m && m <= 1000000000);
    assert(-1000000000 <= a && a <= 1000000000);
    assert(-1000000000 <= b && b <= 1000000000);
    ll qa = a/m-(a%m < 0), qb = b/m-(b%m < 0);
    __int128 ans = (__int128)qa*n*(n-1)/2+(__int128)qb*n;
    a -= qa*m; b -= qb*m;
    for (;;) {
        ans += (__int128)(a/m)*n*(n-1)/2+(__int128)(b/m)*n;
        a %= m; b %= m;
        ll y = a*n+b;
        if (y < m) return ans;
        n = y/m; b = y%m; swap(m,a);
    }
}
__int128 modSum(ll n, ll m, ll a, ll b) {
    __int128 floors = floorSum(n,m,a,b);
    return (__int128)a*n*(n-1)/2+(__int128)b*n-(__int128)m*floors;
}
