/**
 * Author: Simon Lindholm; team adaptation from Andre notebook
 * License: CC0
 * Summary: Range add/set and maximum on a huge integer domain.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item SparseLazySegTree(n) represents n zeros on $[0,n)$,
 *  $0<n\le LLONG\_MAX$. add/set(l,r,x) add/assign x;
 *  query(l,r) returns maximum, LLONG\_MIN for an empty interval.
 *  Useful when coordinates arrive online and cannot be compressed first.
 *  \item Nodes are created only when descending; child IDs index t.
 *  hasSet distinguishes an assignment from an addition (no reserved value).
 *  push creates children and sends pending tags; modify/query recurse.
 *  Public indices are ll because the domain can be large.
 *  \item Operations $O(\log n)$; after q operations $O(q\log n)$ space.
 *  Queries may allocate children. Stored values must fit ll; cumulative
 *  lazy tags use \_\_int128, so cancelling large additions remain safe.
 *  \end{itemize}
 * Usage: SparseLazySegTree st(1000000000000LL);
 *  st.add(10,20,3); ll best=st.query(0,30); // 3
 */
#pragma once

struct SparseLazySegTree {
	struct Node {
		int left = -1, right = -1;
		ll value = 0;
		__int128 tag = 0;
		bool hasSet = false;
	};
	ll n;
	vec<Node> t = vec<Node>(1);
	SparseLazySegTree(ll N) : n(N) { assert(n > 0); }
	void apply(int p, __int128 x, bool replace) {
		__int128 value = replace ? x : (__int128)t[p].value+x;
		assert(LLONG_MIN <= value && value <= LLONG_MAX);
		t[p].value = ll(value);
		if (replace) { t[p].tag = x; t[p].hasSet = true; }
		else t[p].tag += x;
	}
	void push(int p) {
		if (t[p].left < 0) {
			int id = sz(t); t.resize(id+2);
			t[p].left = id; t[p].right = id+1;
		}
		apply(t[p].left,t[p].tag,t[p].hasSet);
		apply(t[p].right,t[p].tag,t[p].hasSet);
		t[p].tag = 0; t[p].hasSet = false;
	}
	void modify(ll l, ll r, ll x, bool replace, int p, ll a, ll b) {
		if (r <= a || b <= l || l == r) return;
		if (l <= a && b <= r) { apply(p,x,replace); return; }
		push(p); ll m = a+(b-a)/2;
		modify(l,r,x,replace,t[p].left,a,m);
		modify(l,r,x,replace,t[p].right,m,b);
		t[p].value = max(t[t[p].left].value,t[t[p].right].value);
	}
	ll query(ll l, ll r, int p, ll a, ll b) {
		if (r <= a || b <= l || l == r) return LLONG_MIN;
		if (l <= a && b <= r) return t[p].value;
		push(p); ll m = a+(b-a)/2;
		return max(query(l,r,t[p].left,a,m),query(l,r,t[p].right,m,b));
	}
	void check(ll l, ll r) const { assert(0 <= l && l <= r && r <= n); }
	void add(ll l, ll r, ll x) { check(l,r); modify(l,r,x,false,0,0,n); }
	void set(ll l, ll r, ll x) { check(l,r); modify(l,r,x,true,0,0,n); }
	ll query(ll l, ll r) { check(l,r); return query(l,r,0,0,n); }
};
