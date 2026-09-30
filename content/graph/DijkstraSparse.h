/**
 * Author: a
 * Date: a
 * License: CC0
 * Source: a
 * Description: Dijkstra for sparse graphs, when $M$ is much smaller than $N^2$
 * Time: O(N \log N + M)
 * Status: works
 */

vector<vector<pair<int, int>>> adj;

void dijkstra(int s, vector<int> & d, vector<int> & p) {
    int n = adj.size();
    d.assign(n, INF);
    p.assign(n, -1);

    d[s] = 0;
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> q;
    q.push({0, s});
    while(!q.empty()){
        auto [d_v, v] = q.top();
        q.pop();

        if(d_v != d[v]) continue;

        for(auto [to, len]:adj[v]){
            if(d[v] + len < d[to]){
                d[to] = d[v] + len;
                p[to] = v;
                q.push({d[to], to});
            }
        }
    }
}