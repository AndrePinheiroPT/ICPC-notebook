/**
 * Author: jet notebook
 * Summary: Union area of axis-aligned rectangles
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *   \item Exact union area of integer rectangles
 *   (x1,y1,x2,y2), interpreted as $[x1,x2)\times[y1,y2)$.
 *   Ordered bounds; empty rectangles are ignored.
 *   \item Lengths, products and total area must fit ll.
 *   Input unchanged; ll coordinates.
 *   \item $O(n\log n)$ time, $O(n)$ extra space.
 *   \end{itemize}
 * Usage: ll area = rectUnion(rect);
 *  // rect: vec<array<ll,4>>, {x1,y1,x2,y2}
 */
#pragma once

ll rectUnion(const vec<array<ll,4>>& rect) {
	vec<tuple<ll,ll,ll,int>> ev;
	vec<ll> y;
	for (auto [x1,y1,x2,y2] : rect)
		if (x1 < x2 && y1 < y2) {
			ev.pb({x1,y1,y2,1});
			ev.pb({x2,y1,y2,-1});
			y.pb(y1); y.pb(y2);
		}
	if (ev.empty()) return 0;
	sort(ev.begin(),ev.end());
	sort(y.begin(),y.end());
	y.erase(unique(y.begin(),y.end()),y.end());
	int n = sz(y)-1;
	vec<int> cover(4*n);
	vec<ll> len(4*n);
	auto add = [&](auto&& self, int p,int l,int r,
			int a, int b, int d)->void {
		if (b <= l || r <= a) return;
		if (a <= l && r <= b) cover[p] += d;
		else {
			int m = l+(r-l)/2;
			self(self,2*p,l,m,a,b,d);
			self(self,2*p+1,m,r,a,b,d);
		}
		len[p] = cover[p] ? y[r]-y[l] :
			(r-l == 1 ? 0 : len[2*p]+len[2*p+1]);
	};
	ll area = 0;
	ll prev = get<0>(ev[0]);
	for (auto [x,y1,y2,d] : ev) {
		area += len[1]*(x-prev);
		int a = int(lower_bound(y.begin(),y.end(),y1)
			-y.begin());
		int b = int(lower_bound(y.begin(),y.end(),y2)
			-y.begin());
		add(add,1,0,n,a,b,d); prev = x;
	}
	return area;
}
