/**
 * Author: KACTL contributors; team adaptation from Andre notebook
 * Summary: Implicit treap: editable sequence with split/merge.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item ImplicitTreap starts empty. insert(pos,x) inserts before pos;
 *  erase(l,r), reverse(l,r) and query(l,r) remove/reverse/sum $[l,r)$.
 *  move(l,r,k) removes that block and inserts it before index k of the
 *  REMAINING sequence. size() is current length; values() materialises it.
 *  \item Nodes are IDs in t (0=null). cnt and sum aggregate each subtree;
 *  rev is pending reversal. split(v,k) separates the first k values;
 *  merge(a,b) concatenates disjoint trees; push propagates reversal,
 *  pull updates aggregates. Normally use the public sequence methods.
 *  \item Expected $O(\log n)$ per operation; values $O(n)$.
 *  Recursion depth is expected $O(\log n)$, worst-case $O(n)$.
 *  Random priorities; sums fit ll. Erased nodes remain allocated:
 *  memory $O(I)$ for I total insertions; destroy the object to reclaim.
 *  \end{itemize}
 * Usage: ImplicitTreap t; t.insert(0,3); t.insert(1,7);
 *  t.reverse(0,2); auto a=t.values(); // {7,3}
 */
#pragma once

struct ImplicitTreap {
	struct Node {
		int l = 0, r = 0, cnt = 1;
		ll val, sum;
		uint64_t priority;
		bool rev = false;
		Node(ll x, uint64_t p) : val(x), sum(x), priority(p) {}
	};
	vec<Node> t{Node(0,0)};
	int root = 0;
	mt19937_64 rng;
	ImplicitTreap(uint64_t seed = chrono::steady_clock::now()
		.time_since_epoch().count()) : rng(seed) { t[0].cnt = 0; }
	int size() const { return t[root].cnt; }
	void pull(int v) {
		t[v].cnt = 1+t[t[v].l].cnt+t[t[v].r].cnt;
		t[v].sum = t[v].val+t[t[v].l].sum+t[t[v].r].sum;
	}
	void push(int v) {
		if (!v || !t[v].rev) return;
		swap(t[v].l,t[v].r);
		if (t[v].l) t[t[v].l].rev = !t[t[v].l].rev;
		if (t[v].r) t[t[v].r].rev = !t[t[v].r].rev;
		t[v].rev = false;
	}
	pair<int,int> split(int v, int k) {
		if (!v) return {0,0};
		push(v);
		if (t[t[v].l].cnt >= k) {
			auto [a,b] = split(t[v].l,k);
			t[v].l = b; pull(v); return {a,v};
		}
		auto [a,b] = split(t[v].r,k-t[t[v].l].cnt-1);
		t[v].r = a; pull(v); return {v,b};
	}
	int merge(int a, int b) {
		if (!a || !b) return a ? a : b;
		if (t[a].priority > t[b].priority) {
			push(a); t[a].r = merge(t[a].r,b); pull(a); return a;
		}
		push(b); t[b].l = merge(a,t[b].l); pull(b); return b;
	}
	void check(int l, int r) const { assert(0 <= l && l <= r && r <= size()); }
	void insert(int pos, ll x) {
		check(pos,pos);
		auto [a,b] = split(root,pos);
		int v = sz(t); t.emplace_back(x,rng());
		root = merge(merge(a,v),b);
	}
	void erase(int l, int r) {
		check(l,r); auto [a,bc] = split(root,l);
		auto [b,c] = split(bc,r-l); (void)b; root = merge(a,c);
	}
	void reverse(int l, int r) {
		check(l,r); auto [a,bc] = split(root,l);
		auto [b,c] = split(bc,r-l);
		if (b) t[b].rev = !t[b].rev;
		root = merge(a,merge(b,c));
	}
	ll query(int l, int r) {
		check(l,r); auto [a,bc] = split(root,l);
		auto [b,c] = split(bc,r-l); ll ans = t[b].sum;
		root = merge(a,merge(b,c)); return ans;
	}
	void move(int l, int r, int k) {
		check(l,r); assert(0 <= k && k <= size()-(r-l));
		auto [a,bc] = split(root,l); auto [b,c] = split(bc,r-l);
		auto [x,y] = split(merge(a,c),k); root = merge(x,merge(b,y));
	}
	void collect(int v, vec<ll>& a) {
		if (!v) return;
		push(v); collect(t[v].l,a); a.pb(t[v].val); collect(t[v].r,a);
	}
	vec<ll> values() { vec<ll> a; collect(root,a); return a; }
};
