/**
 * Author: Lukas Polacek
 * License: CC0
 * Source: folklore
 * Summary: DSU: merge components and test connectivity.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item UF(n) starts with n separate vertices. join(a,b) returns true only
 *  when two components merge; find(v) returns its representative,
 *  sameSet(a,b) tests connectivity, size(v) gives the component size.
 *  \item e[root] is the negative size; other entries are DSU parents
 *  (internal convention, not tree parents). Union by size and path compression.
 *  \item Build/space $O(n)$; operations amortised $O(\alpha(n))$.
 *  \end{itemize}
 * Usage: UF uf(3); uf.join(0,1); bool yes = uf.sameSet(0,1);
 */
#pragma once

struct UF {
	vec<int> e;
	UF(int n) : e(n, -1) {}
	bool sameSet(int a, int b) { return find(a) == find(b); }
	int size(int x) { return -e[find(x)]; }
	int find(int x) {
		if (e[x] < 0) return x;
		return e[x] = find(e[x]); // comprime o caminho
	}
	bool join(int a, int b) {
		a = find(a); b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b]; e[b] = a;
		return true;
	}
};
