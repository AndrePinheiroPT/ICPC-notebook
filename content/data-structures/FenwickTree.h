/**
 * Author: Lukas Polacek
 * Date: 2009-10-30
 * License: CC0
 * Source: folklore/TopCoder
 * Description: Fenwick tree, supports 0-index.
 * Time: Both operations are $O(\log N)$.
 * Status: Stress-tested
 */
struct BIT{
    vector<ll> bit; ll n;
    BIT(ll n) : n(n+1), bit(n+5, 0) {}
    void update(ll x, ll delta){ for(++x; x<=n; x+=x&-x) bit[x] += delta; }
    ll pre(ll x){
        ll sum = 0; 
        for(++x; x; x-=x&-x) sum += bit[x];
        return sum;
    }
    ll query(ll l, ll r){ return pre(r)-pre(l-1); }
};
