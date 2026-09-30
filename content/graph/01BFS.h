/**
 * Author: 
 * Date: 
 * License: CC0
 * Source:
 * Description: Single-source shortest path, but weights can only be 0 or 1.
 * Time: O(M)
 * Status: works
 */

vector<int> d(n, INF);
d[s] = 0;
deque<int> q;
q.push_front(s);
while(!q.empty()){
    int v = q.front();
    q.pop_front();
    for(auto [u,w] : adj[v]){
        int u = edge.first;
        int w = edge.second;
        if (d[v] + w < d[u]) {
            d[u] = d[v] + w;
            if(w == 1) q.push_back(u);
            else       q.push_front(u);
        }
    }
}