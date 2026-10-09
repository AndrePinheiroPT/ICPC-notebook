/**
 * Author: jet, Goncalo and Andre notebooks; team adaptation
 * Summary: Range add/set, range sum and range maximum.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Construct from vec<T> a (T defaults to ll). add(l,r,x) adds x
 *  to every value in $[l,r)$; set(l,r,x) overwrites that range.
 *  query(l,r) returns its sum; queryMax(l,r) its maximum.
 *  Empty ranges return 0 / numeric\_limits<T>::lowest().
 *  \item sum/mx store aggregates. assigned marks a pending assignment;
 *  tag then stores its value, otherwise an addition. apply updates a node,
 *  push passes pending work to children, pull recomputes the parent.
 *  Use the short public methods, not the recursive overloads.
 *  \item Build/space $O(n)$; operations $O(\log n)$. T must be an ordered
 *  numeric type; all intermediate sums, products and CUMULATIVE lazy
 *  tags must fit T, even when the final individual values fit.
 *  \end{itemize}
 * Usage: LazySegTree st(vec<ll>{1,2,3}); st.set(0,2,5);
 *  st.add(1,3,2); ll total=st.query(0,3); // 17
 */
#pragma once

template<class T = ll> struct LazySegTree {
	int n;
	vec<T> sum, mx, tag;
	vec<bool> assigned;
	LazySegTree(const vec<T>& a) : n(sz(a)), sum(4*n),
		mx(4*n), tag(4*n), assigned(4*n) {
		if (n) build(1,0,n,a);
	}
	void pull(int p) {
		sum[p] = sum[2*p]+sum[2*p+1];
		mx[p] = max(mx[2*p],mx[2*p+1]);
	}
	void build(int p, int l, int r, const vec<T>& a) {
		if (r-l == 1) { sum[p] = mx[p] = a[l]; return; }
		int m = l+(r-l)/2;
		build(2*p,l,m,a); build(2*p+1,m,r,a); pull(p);
	}
	void apply(int p, int len, T x, bool replace) {
		if (replace) {
			sum[p] = x*len; mx[p] = tag[p] = x;
			assigned[p] = true;
		} else {
			sum[p] += x*len; mx[p] += x; tag[p] += x;
		}
	}
	void push(int p, int l, int r) {
		if (!assigned[p] && tag[p] == 0) return;
		int m = l+(r-l)/2;
		apply(2*p,m-l,tag[p],assigned[p]);
		apply(2*p+1,r-m,tag[p],assigned[p]);
		tag[p] = 0; assigned[p] = false;
	}
	void update(int l, int r, T x, bool replace,
			int p, int a, int b) {
		if (r <= a || b <= l || l == r) return;
		if (l <= a && b <= r) { apply(p,b-a,x,replace); return; }
		push(p,a,b);
		int m = a+(b-a)/2;
		update(l,r,x,replace,2*p,a,m);
		update(l,r,x,replace,2*p+1,m,b); pull(p);
	}
	T query(int l, int r, bool maximum, int p, int a, int b) {
		if (r <= a || b <= l || l == r)
			return maximum ? numeric_limits<T>::lowest() : T(0);
		if (l <= a && b <= r) return maximum ? mx[p] : sum[p];
		push(p,a,b); int m = a+(b-a)/2;
		T x = query(l,r,maximum,2*p,a,m);
		T y = query(l,r,maximum,2*p+1,m,b);
		return maximum ? max(x,y) : x+y;
	}
	void check(int l, int r) const {
		assert(0 <= l && l <= r && r <= n);
	}
	void add(int l, int r, T x) {
		check(l,r); update(l,r,x,false,1,0,n);
	}
	void set(int l, int r, T x) {
		check(l,r); update(l,r,x,true,1,0,n);
	}
	T query(int l, int r) {
		check(l,r); return query(l,r,false,1,0,n);
	}
	T queryMax(int l, int r) {
		check(l,r); return query(l,r,true,1,0,n);
	}
};
