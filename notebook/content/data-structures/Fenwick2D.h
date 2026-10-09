/**
 * Author: jet notebook
 * Summary: Sparse point additions and rectangle sums.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use for point additions and rectangle sums
 *  with large/sparse coordinates. Pass ALL possible
 *  update points first; values initially zero. Later,
 *  add(x,y,v) adds v only at a registered point.
 *  \item query(X,Y) sums points with x<X and y<Y;
 *  query(x1,y1,x2,y2) sums $[x_1,x_2)\times[y_1,y_2)$.
 *  Query boundaries need not be registered. xs/ys compress
 *  coordinates; pos finds an index, ft holds Fenwick
 *  trees. Both tree levels use 1-based internal indices.
 *  \item m registered points: build $O(m\log^2 m)$,
 *  space $O(m\log m)$, add/query $O(\log^2 m)$.
 *  T is the sum type, C the coordinate type; sums fit T.
 *  \end{itemize}
 * Usage: Fenwick2D<ll,ll> f(points); f.add(x,y,7);
 *  ll sum = f.query(x1,y1,x2,y2);
 */
#pragma once
#include "Fenwick.h"

template<class T = ll, class C = int>
struct Fenwick2D {
	vec<C> xs;
	vec<vec<C>> ys;
	vec<Fenwick<T>> ft;
	static int pos(const vec<C>& v, C x) {
		return int(lower_bound(v.begin(), v.end(), x)
			- v.begin());
	}
	Fenwick2D(vec<pair<C,C>> p = {}) {
		sort(p.begin(), p.end());
		p.erase(unique(p.begin(), p.end()), p.end());
		for (auto [x,y] : p) xs.pb(x);
		xs.erase(unique(xs.begin(), xs.end()),
			xs.end());
		ys.resize(xs.size()+1);
		for (auto [x,y] : p)
			for (ll i = ll(pos(xs,x))+1; i <= sz(xs);
				i += i & -i)
				ys[i].pb(y);
		for (auto& v : ys) {
			sort(v.begin(), v.end());
			v.erase(unique(v.begin(), v.end()),
				v.end());
			ft.emplace_back(sz(v));
		}
	}
	void add(C x, C y, T delta) {
		int at = pos(xs,x);
		assert(at < sz(xs) && xs[at] == x);
		for (ll i = ll(at)+1; i <= sz(xs); i += i & -i) {
			int j = pos(ys[i],y);
			assert(j < sz(ys[i]) && ys[i][j] == y);
			ft[i].add(j, delta);
		}
	}
	T query(C x, C y) const {
		T ans{};
		for (int i = pos(xs,x); i; i &= i-1)
			ans += ft[i].query(pos(ys[i],y));
		return ans;
	}
	T query(C x1, C y1, C x2, C y2) const {
		assert(x1 <= x2 && y1 <= y2);
		return query(x2,y2) - query(x1,y2)
			- query(x2,y1) + query(x1,y1);
	}
};
