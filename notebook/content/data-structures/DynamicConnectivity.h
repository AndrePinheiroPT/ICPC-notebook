/**
 * Author: jet notebook
 * Summary: Offline connectivity with edge additions and removals.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use for graph edges added AND removed,
 *  when all operations are known in advance. If there
 *  are only removals, reverse time and use ordinary UF.
 *  \item Construct (n,Q): vertices and time steps.
 *  add(l,r,u,v) makes edge u-v active at $l,\ldots,r-1$.
 *  Give one interval per active copy of repeated edges.
 *  \item run(answer) calls your answer(t,uf) at each
 *  time; save answers there using find/size, without
 *  changing uf. run returns nothing. s groups lifetimes;
 *  dfs temporarily merges edges, visits times, then undoes.
 *  \item I intervals: $O(n+Q+I\log Q\log n)$ plus
 *  answer calls; space $O(n+Q+I\log Q)$.
 *  \end{itemize}
 * Usage: dc.run([&](int t, RollbackUF& uf) {
 *    ans[t] = uf.find(u[t]) == uf.find(v[t]);
 *  }); // dc was built and intervals added above
 */
#pragma once
#include "UnionFindRollback.h"

struct DynamicConnectivity {
	int q, n = 1;
	RollbackUF uf;
	vec<vec<pair<int,int>>> s;
	DynamicConnectivity(int N, int Q)
		: q(Q), uf(N) {
		while (n < q) n *= 2;
		s.resize(2*n);
	}
	void add(int l, int r, int u, int v) {
		assert(0 <= l && l <= r && r <= q);
		for (l += n, r += n; l < r; l /= 2, r /= 2) {
			if (l & 1) s[l++].pb({u,v});
			if (r & 1) s[--r].pb({u,v});
		}
	}
	template<class F> void dfs(int k, F& f) {
		int t = uf.time();
		for (auto [u,v] : s[k]) uf.join(u,v);
		if (k >= n) {
			if (k-n < q) f(k-n,uf);
		} else {
			dfs(2*k,f); dfs(2*k+1,f);
		}
		uf.rollback(t);
	}
	template<class F> void run(F f) { dfs(1,f); }
};
