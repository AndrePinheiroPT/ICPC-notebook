/**
 * Author: urmom
 * Date: 2017-10-31
 * License: CC0
 * Source: folklore
 * Description: Point update and query functions of a segment tree.
 * Usage: up(up,1,0,n-1,i,v); query(query,1,0,n-1,l,r);
 * Time: O(\log N)
 * Status: stress-tested
 */


vec<ll> seg(4*n,0); 

// Point update
auto up = [&](auto &up, ll v, ll L, ll R, ll pos, ll val) -> void {
    if(L == R) seg[v]=val;
    else{
        ll mid = (L+R)/2;
        if(pos <= mid) up(up, v*2, L, mid, pos, val);
        else up(up, v*2+1, mid+1, R, pos, val);
        seg[v]=seg[v*2]+seg[v*2+1];
    }
};


// Query
auto query = [&](auto &query, ll v, ll L, ll R, ll lx, ll rx) -> ll { // [L,R]
    if(lx > rx) return 0;
    if(L == lx && R == rx) return seg[v];

    ll mid = (L+R)/2;
    return 
        query(query,2*v,L,mid,lx,min(rx,mid))+
        query(query,2*v+1,mid+1,R,max(lx,mid+1),rx)
    ;
};