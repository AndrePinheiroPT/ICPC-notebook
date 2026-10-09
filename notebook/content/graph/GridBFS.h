/**
 * Author: jet notebook
 * Summary: Shortest paths through a grid
 * Description: Rectangular nonempty character grid; wall blocks movement, all other cells are free. Sources are valid free (row,column) pairs; moves are orthogonal with unit cost. Returns (dist,parent) indexed by id=row*C+column; decode with id/C and id\%C. dist=-1 for walls/unreachable cells; parent=-1 for sources/unreachable cells. Several sources give distance to the nearest. Follow parents to recover a path. $RC\le INT\_MAX$; time $O(RC+|src|)$, space $O(RC)$.
 * Usage: vec<string> a{"...",".#.","..."};
 *    auto [d,p] = gridBfs(a,{{0,0}});
 *    // d[2*3+2]=4; p contains the path.
 
 */
#pragma once

pair<vec<int>,vec<int>> gridBfs(const vec<string>& a,
		const vec<pair<int,int>>& src,
		char wall = '#') {
	assert(!a.empty());
	int R = sz(a), C = sz(a[0]);
	assert(C > 0 && (ll)R*C <= INT_MAX);
	rep(r,0,R) assert(sz(a[r]) == C);
	vec<int> dist(R*C,-1), parent(R*C,-1), q;
	for (auto [r,c] : src) {
		assert(0 <= r && r < R && 0 <= c && c < C);
		assert(a[r][c] != wall);
		int v = r*C+c;
		if (dist[v] < 0) dist[v] = 0, q.pb(v);
	}
	const int dr[4] = {-1,1,0,0};
	const int dc[4] = {0,0,-1,1};
	rep(i,0,sz(q)) {
		int v = q[i], r = v/C, c = v%C;
		rep(k,0,4) {
			int nr = r+dr[k], nc = c+dc[k];
			if (nr < 0 || nr >= R || nc < 0 || nc >= C)
				continue;
			int u = nr*C+nc;
			if (a[nr][nc] == wall || dist[u] >= 0) continue;
			dist[u] = dist[v]+1; parent[u] = v;
			q.pb(u);
		}
	}
	return {dist,parent};
}
