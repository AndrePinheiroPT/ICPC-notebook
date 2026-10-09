/**
 * Author: Lukas Polacek, Simon Lindholm
 * Date: 2019-12-26
 * License: CC0
 * Source: folklore
 * Summary: DSU snapshots: temporarily merge components, then undo.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use when merges must be undone. Start with
 *  n separate vertices; find(v) gives the root, size(v)
 *  the group size; join(a,b) returns true if merged.
 *  \item Save t=time(), then rollback(t) undoes later
 *  merges. t must be a snapshot of the current history.
 *  e[root]=-size; st saves changed entries for undo.
 *  No path compression; unchanged joins save nothing.
 *  \item Find/join $O(\log n)$, snapshot $O(1)$,
 *  undo k joins $O(k)$; build/space $O(n)$.
 *  \end{itemize}
 * Usage: RollbackUF uf(n); int t = uf.time();
 *  uf.join(a,b); uf.rollback(t); // undo that merge
 */
#pragma once

struct RollbackUF {
	vec<int> e; vec<pair<int,int>> st;
	RollbackUF(int n) : e(n, -1) {}
	int size(int x) { return -e[find(x)]; }
	int find(int x) {
		while (e[x] >= 0) x = e[x];
		return x;
	}
	int time() { return sz(st); }
	void rollback(int t) {
		assert(0 <= t && t <= time());
		while (time() > t) {
			auto [i, old] = st.back(); st.pop_back();
			e[i] = old;
		}
	}
	bool join(int a, int b) {
		a = find(a); b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.pb({a, e[a]});
		st.pb({b, e[b]});
		e[a] += e[b]; e[b] = a;
		return true;
	}
};
