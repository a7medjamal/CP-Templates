#include <bits/stdc++.h>
using namespace std;

// Heavy-Light Decomposition for Tree Path Queries
class HeavyLightDecomposition {
public:
    int n, timer;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, size;
    vector<long long> tree;
    
    HeavyLightDecomposition(int n) : n(n), timer(0) {
        adj.resize(n);
        parent.resize(n);
        depth.resize(n);
        heavy.resize(n, -1);
        head.resize(n);
        pos.resize(n);
        size.resize(n);
        tree.resize(4 * n);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    int dfs(int v, int p = -1) {
        size[v] = 1;
        parent[v] = p;
        int maxChildSize = 0;
        
        for (int u : adj[v]) {
            if (u != p) {
                depth[u] = depth[v] + 1;
                int childSize = dfs(u, v);
                size[v] += childSize;
                if (childSize > maxChildSize) {
                    maxChildSize = childSize;
                    heavy[v] = u;
                }
            }
        }
        return size[v];
    }
    
    void decompose(int v, int h, int p = -1) {
        head[v] = h;
        pos[v] = timer++;
        
        if (heavy[v] != -1) {
            decompose(heavy[v], h, v);
        }
        
        for (int u : adj[v]) {
            if (u != p && u != heavy[v]) {
                decompose(u, u, v);
            }
        }
    }
    
    void build() {
        dfs(0);
        decompose(0, 0);
    }
    
    void updateTree(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid) {
                updateTree(2 * node, start, mid, idx, val);
            } else {
                updateTree(2 * node + 1, mid + 1, end, idx, val);
            }
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }
    
    long long queryTree(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        
        int mid = (start + end) / 2;
        return queryTree(2 * node, start, mid, l, r) + 
               queryTree(2 * node + 1, mid + 1, end, l, r);
    }
    
    void update(int v, long long val) {
        updateTree(1, 0, n - 1, pos[v], val);
    }
    
    long long query(int u, int v) {
        long long result = 0;
        
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) swap(u, v);
            result += queryTree(1, 0, n - 1, pos[head[v]], pos[v]);
            v = parent[head[v]];
        }
        
        if (depth[u] > depth[v]) swap(u, v);
        result += queryTree(1, 0, n - 1, pos[u], pos[v]);
        
        return result;
    }
};

// Example usage:
/*
int main() {
    HeavyLightDecomposition hld(6);
    hld.addEdge(0, 1);
    hld.addEdge(0, 2);
    hld.addEdge(1, 3);
    hld.addEdge(1, 4);
    hld.addEdge(2, 5);
    hld.build();
    
    hld.update(3, 10);
    cout << "Path sum from 3 to 5: " << hld.query(3, 5) << endl;
    
    return 0;
}
*/