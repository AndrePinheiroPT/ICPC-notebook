/**
 * Author: Johan Sannemo, pajenegod
 * License: CC0
 * Source: KACTL; consultas de mínimo.
 * Summary: Sparse table: minimum on a static range.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Construct RMQ(a); query(l,r) returns the minimum on nonempty
 *  $[l,r)$. Store (value,index) pairs to retrieve the first minimum position.
 *  No updates. jmp[k][i] covers $2^k$ values starting at i.
 *  \item Change both min calls to max for maxima. Overlapping blocks require
 *  an idempotent operation: this query formula does not work for sums.
 *  \item Build/space $O(n\log n)$, query $O(1)$; $n<2^{30}$.
 *  \end{itemize}
 * Usage: RMQ rmq(vec<ll>{5,1,3}); ll x=rmq.query(0,2); // 1
 */
#pragma once

template<class T = ll> struct RMQ {
	vec<vec<T>> jmp;
	RMQ(const vec<T>& a) : jmp(1,a) {
		for (int k = 1; (1 << k) <= sz(a); ++k) {
			jmp.emplace_back(sz(a) - (1 << k) + 1);
			rep(i,0,sz(jmp[k]))
				jmp[k][i] = min(jmp[k-1][i],
					jmp[k-1][i + (1 << (k-1))]);
		}
	}
	T query(int l, int r) const {
		assert(0 <= l && l < r && r <= sz(jmp[0]));
		int k = __lg(r-l);
		return min(jmp[k][l], jmp[k][r - (1 << k)]);
	}
};
