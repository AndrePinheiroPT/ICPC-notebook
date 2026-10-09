/**
 * Author: jet notebook
 * Summary: Satisfy clauses with at most two literals
 * Description: TwoSAT(n): variables [0,n), literal 2*v means v=true, 2*v+1 means v=false; negate with xor 1. either(a,b) adds a OR b; either(a,a) forces a; implication a=>b is either(a xor 1,b). XOR adds (a OR b) and (not-a OR not-b). solve() returns false if impossible; after true, answer[v] is one valid 0/1 assignment. Re-solving after adding clauses is allowed. Clauses of 3+ literals are unsupported; does not count solutions or optimise true variables. Uses iterative Kosaraju. Add $O(1)$ amortised; solve $O(V+E)$ time/space.
 * Usage: TwoSAT s(2);
 *    s.either(0,2); s.either(1,3); // exactly one
 *    s.either(0,0);
 *    bool ok=s.solve(); // answer={1,0}
 
 */
#pragma once
#include "StrongComponents.h"

struct TwoSAT {
	vec<vec<int>> g;
	vec<int> answer;
	TwoSAT(int n) : g(2*n) {}
	void either(int a, int b) {
		g[a^1].pb(b);
		g[b^1].pb(a);
	}
	bool solve() {
		vec<int> comp = kosaraju(g);
		answer.resize(sz(g)/2);
		rep(v,0,sz(answer)) {
			int a = comp[2*v], b = comp[2*v+1];
			if (a == b) return false;
			answer[v] = a > b;
		}
		return true;
	}
};
