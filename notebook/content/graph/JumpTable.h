/**
 * Author: jet notebook
 * Summary: Repeated successors and k-th ancestors
 * Description: next[v] is the successor, -1 if absent; it may contain cycles. jmp[k][v] stores the successor after $2^k$ steps. jump(v,k) returns the destination after k steps, or -1 after leaving the graph. Choose $1\le lg\le64$ and $steps<2^{lg}$; lg=64 supports all unsigned 64-bit step counts. For a tree pass parents with parent[root]=-1. Build/space $O(V\,lg)$, each jump $O(lg)$.
 * Usage: JumpTable t(vec<int>{1,2,0},64);
 *    int v=t.jump(0,5); // 2
 
 */
#pragma once

using ull = unsigned long long;

struct JumpTable {
	vec<vec<int>> jmp;
	JumpTable(const vec<int>& next, int lg)
		: jmp(lg,next) {
		assert(1 <= lg && lg <= 64);
		for (int p : next) assert(-1<=p && p<sz(next));
		rep(j,1,lg)
			rep(v,0,sz(next)) {
				int p = jmp[j-1][v];
				jmp[j][v] = p < 0 ? -1 : jmp[j-1][p];
			}
	}
	int jump(int v, ull k) const {
		assert(-1<=v && v<sz(jmp[0]));
		assert(sz(jmp) == 64 || (k >> sz(jmp)) == 0);
		for (int j = 0; k && v >= 0; ++j, k >>= 1)
			if (k & 1) v = jmp[j][v];
		return v;
	}
};
