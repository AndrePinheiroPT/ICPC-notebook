/**
 * Author: Caio
 * Date: 2015-02-06
 * License: CC0
 * Source: Folklore
 * Description: Normal centroid decomposition algorithm.
 * Time: $O(N \log N)$
 * Status: not tested
 */

struct Centroid{
    int n;
    vector<int> used, pai, sub;
    vector<vector<int>> gr;
    Centroid(int n) : n(n), used(n+1), pai(n+1), sub(n+1), gr(n+1) {}
    void add_edge(int v, int u){
        gr[v].push_back(u);
        gr[u].push_back(v);
    }

    int dfs_sz(int x, int p=0){
        sub[x]=1;
        for(int i:gr[x]){
            if(i==p || used[i]) continue;
            sub[x]+=dfs_sz(i, x);
        }
        return sub[x];
    }

    int find_c(int x, int total, int p=0){
        for(int i:gr[x]){
            if(i==p || used[i]) continue;
            if(2*sub[i]>total) return find_c(i, total, x);
        }
        return x;
    }

    void build(int x=1, int p=0){
        int c=find_c(x, dfs_sz(x)); 
        
        //do something

        used[c]=1;
        pai[c]=p;
        for(int i:gr[c]){
            if(!used[i]) build(i, c);
        }
    }
};