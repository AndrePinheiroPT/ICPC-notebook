/**
 * Author: Simon Lindholm
 * Date: 2015-02-23
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/Bellman-Ford_algorithm
 * Description: Calculates shortest paths from $s$ in a graph that might have negative edge weights.
 * Unreachable nodes get dist = inf; nodes reachable through negative-weight cycles get dist = -inf.

 * Time: O(VE)
 * Status: Tested on kattis:shortestpath3
 */
#pragma once

void BellmanFord(int n, int src, vector<pair<int, ll>> *edges, vector<ll>& dist, set<int>& negCycle) {
	fill(all(dist), INF);
	dist[src] = 0;
	vector<pair<ll, pair<int, int>>> e;
	for (int i = 0; i < n; i++) {
		for (auto j : edges[i]) {
			e.pb({j.ss, {i, j.ff}});
		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (auto j : e) {
			dist[j.ss.ss] = min(dist[j.ss.ss], dist[j.ss.ff] + j.ff);
		}
	}
	//checking negative cycle
	for (int i = 0; i < n; i++) {
		for (auto j : e) {
			if (dist[j.ss.ss] > dist[j.ss.ff] + j.ff) {
				dist[j.ss.ss] = dist[j.ss.ff] + j.ff;
				negCycle.insert(j.ss.ss);
			}
		}
	}
}