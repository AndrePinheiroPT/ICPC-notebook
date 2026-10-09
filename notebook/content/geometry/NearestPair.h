/**
 * Author: jet notebook
 * Summary: Closest pair of input points
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *   \item Returns squared distance and two input IDs.
 *   Fewer than two points gives (0,(-1,-1)).
 *   Ties arbitrary; input unchanged, duplicates valid.
 *   \item Point arithmetic must fit; floating-point
 *   values must remain finite. No epsilon or sqrt.
 *   \item $O(n\log n)$ time, $O(n)$ extra space.
 *   \end{itemize}
 * Usage: auto [d2,ids] = nearestPair(p);
 *  // p: input points; ids: pair of 0-based IDs
 */
#pragma once
#include "Predicates.h"

template<class T>
auto nearestPair(const vec<Point<T>>& p) {
	if (sz(p) < 2) return pair{T(0),pair{-1,-1}};
	using Item = pair<Point<T>,int>;
	vec<Item> a, tmp(sz(p));
	rep(i,0,sz(p))
		a.pb({p[i],i});
	sort(a.begin(),a.end());
	T best = (p[0]-p[1]).dist2();
	pair<int,int> ids{0,1};
	auto test = [&](const Item& u, const Item& v) {
		T d = (u.first-v.first).dist2();
		if (d < best)
			best = d, ids = {u.second,v.second};
	};
	auto byY = [](const Item& u, const Item& v) {
		return u.first.y < v.first.y;
	};
	auto go = [&](auto&& self, int l,int r)->void {
		if (r-l <= 3) {
			rep(i,l,r)
				rep(j,i+1,r)
					test(a[i],a[j]);
			sort(a.begin()+l,a.begin()+r,byY);
			return;
		}
		int m = l+(r-l)/2;
		T x = a[m].first.x;
		self(self,l,m); self(self,m,r);
		merge(a.begin()+l,a.begin()+m,
			a.begin()+m,a.begin()+r,tmp.begin()+l,byY);
		copy_n(tmp.begin()+l,r-l,a.begin()+l);
		int k = 0;
		rep(i,l,r) {
			T dx = a[i].first.x-x;
			if (dx*dx >= best) continue;
			for (int j = k; j--;) {
				T dy = a[i].first.y-tmp[j].first.y;
				if (dy*dy >= best) break;
				test(a[i],tmp[j]);
			}
			tmp[k++] = a[i];
		}
	};
	go(go,0,sz(a));
	return pair{best,ids};
}
