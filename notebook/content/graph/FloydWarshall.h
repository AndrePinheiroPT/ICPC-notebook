/**
 * Author: Simon Lindholm
 * License: CC0
 * Summary: All-pairs shortest paths, including negative cycles
 * Description: Square ll matrix m: m[u][v]=edge cost, FW::inf if absent; take the minimum of parallel edges. Replaces m with shortest distances, with diagonal at most 0. inf means unreachable; -inf means a path through a negative cycle. Allows negative edges; suitable for small V. Require $V\max|w|<inf=2^{62}$; time $O(V^3)$, extra space $O(1)$.
 * Usage: vec<vec<ll>> d(3,vec<ll>(3,FW::inf));
 *    d[0][1]=4; d[1][2]=-2;
 *    FW::floydWarshall(d); // d[0][2]=2
 
 */
#pragma once

namespace FW {
const ll inf = 1LL << 62;
void floydWarshall(vec<vec<ll>>& m) {
	int n = sz(m);
	rep(i,0,n)
		m[i][i] = min(m[i][i], 0LL);
	rep(k,0,n)
		rep(i,0,n)
			rep(j,0,n)
				if (m[i][k] != inf &&
						m[k][j] != inf) {
					auto newDist = max(
						m[i][k] + m[k][j], -inf);
					m[i][j] = min(m[i][j], newDist);
				}
	rep(k,0,n) if (m[k][k] < 0)
		rep(i,0,n)
			rep(j,0,n)
				if (m[i][k] != inf &&
						m[k][j] != inf)
					m[i][j] = -inf;
}
} // namespace FW
