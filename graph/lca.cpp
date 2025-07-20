#include <bits/stdc++.h>
using namespace std;

// Lowest Common Ancestor (LCA) Algorithms
// Multiple implementations for different use cases and constraints

namespace LCA {
    
    // Binary Lifting LCA - O(log n) query, O(n log n) preprocessing
    class BinaryLifting {
    private:
        int n, LOG;
        vector<vector<int>> adj;
        vector<vector<int>> up;
        vector<int> depth;
        
        void dfs(int v, int p) {
            up[v][0] = p;
            for (int i = 1; i < LOG; i++) {
                up[v][i] = up[up[v][i-1]][i-1];
            }
            
            for (int u : adj[v]) {
                if (u != p) {
                    depth[u] = depth[v] + 1;
                    dfs(u, v);
                }
            }
        }
        
    public:
        BinaryLifting(int n) : n(n) {
            LOG = 0;
            while ((1 << LOG) <= n) LOG++;
            adj.resize(n);
            up.assign(n, vector<int>(LOG));
            depth.resize(n);
        }
        
        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        void preprocess(int root = 0) {
            depth[root] = 0;
            dfs(root, root);
        }
        
        int lca(int a, int b) {
            if (depth[a] < depth[b]) swap(a, b);
            
            int diff = depth[a] - depth[b];
            for (int i = 0; i < LOG; i++) {
                if ((diff >> i) & 1) {
                    a = up[a][i];
                }
            }
            
            if (a == b) return a;
            
            for (int i = LOG - 1; i >= 0; i--) {
                if (up[a][i] != up[b][i]) {
                    a = up[a][i];
                    b = up[b][i];
                }
            }
            
            return up[a][0];
        }
        
        int distance(int a, int b) {
            return depth[a] + depth[b] - 2 * depth[lca(a, b)];
        }
        
        int kthAncestor(int v, int k) {
            if (depth[v] < k) return -1;
            
            for (int i = 0; i < LOG; i++) {
                if ((k >> i) & 1) {
                    v = up[v][i];
                }
            }
            return v;
        }
        
        bool isAncestor(int u, int v) {
            return depth[u] <= depth[v] && lca(u, v) == u;
        }
    };
    
    // Euler Tour + Sparse Table LCA - O(1) query, O(n log n) preprocessing
    class EulerTourLCA {
    private:
        int n;
        vector<vector<int>> adj;
        vector<int> euler, first, depth;
        vector<vector<int>> st;
        
        void dfs(int v, int p, int d) {
            first[v] = (int)euler.size();
            euler.push_back(v);
            depth[v] = d;
            
            for (int u : adj[v]) {
                if (u != p) {
                    dfs(u, v, d + 1);
                    euler.push_back(v);
                }
            }
        }
        
        void buildSparseTable() {
            int m = (int)euler.size();
            int LOG = 0;
            while ((1 << LOG) <= m) LOG++;
            
            st.assign(LOG, vector<int>(m));
            
            for (int i = 0; i < m; i++) {
                st[0][i] = i;
            }
            
            for (int j = 1; j < LOG; j++) {
                for (int i = 0; i + (1 << j) <= m; i++) {
                    int left = st[j-1][i];
                    int right = st[j-1][i + (1 << (j-1))];
                    st[j][i] = (depth[euler[left]] < depth[euler[right]]) ? left : right;
                }
            }
        }
        
        int query(int l, int r) {
            int len = r - l + 1;
            int k = 0;
            while ((1 << (k + 1)) <= len) k++;
            
            int left = st[k][l];
            int right = st[k][r - (1 << k) + 1];
            return (depth[euler[left]] < depth[euler[right]]) ? euler[left] : euler[right];
        }
        
    public:
        EulerTourLCA(int n) : n(n) {
            adj.resize(n);
            first.resize(n);
            depth.resize(n);
        }
        
        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        void preprocess(int root = 0) {
            euler.clear();
            dfs(root, -1, 0);
            buildSparseTable();
        }
        
        int lca(int a, int b) {
            int l = first[a], r = first[b];
            if (l > r) swap(l, r);
            return query(l, r);
        }
        
        int distance(int a, int b) {
            return depth[a] + depth[b] - 2 * depth[lca(a, b)];
        }
    };
    
    // Simple DFS LCA - O(n) query, O(n) preprocessing
    // Good for small trees or when preprocessing time is critical
    class SimpleLCA {
    private:
        int n;
        vector<vector<int>> adj;
        vector<int> parent, depth;
        
        void dfs(int v, int p, int d) {
            parent[v] = p;
            depth[v] = d;
            
            for (int u : adj[v]) {
                if (u != p) {
                    dfs(u, v, d + 1);
                }
            }
        }
        
    public:
        SimpleLCA(int n) : n(n) {
            adj.resize(n);
            parent.resize(n);
            depth.resize(n);
        }
        
        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        void preprocess(int root = 0) {
            dfs(root, -1, 0);
        }
        
        int lca(int a, int b) {
            if (depth[a] < depth[b]) swap(a, b);
            
            // Bring a to the same level as b
            while (depth[a] > depth[b]) {
                a = parent[a];
            }
            
            // Move both up until they meet
            while (a != b) {
                a = parent[a];
                b = parent[b];
            }
            
            return a;
        }
        
        int distance(int a, int b) {
            return depth[a] + depth[b] - 2 * depth[lca(a, b)];
        }
    };
    
    // Heavy-Light Decomposition LCA - O(log^2 n) query, O(n) preprocessing
    // Also supports path queries and updates
    class HeavyLightLCA {
    private:
        int n;
        vector<vector<int>> adj;
        vector<int> parent, depth, heavy, head, pos;
        int timer;
        
        int dfs1(int v, int p) {
            int size = 1;
            int maxChildSize = 0;
            
            for (int u : adj[v]) {
                if (u != p) {
                    parent[u] = v;
                    depth[u] = depth[v] + 1;
                    int childSize = dfs1(u, v);
                    size += childSize;
                    
                    if (childSize > maxChildSize) {
                        maxChildSize = childSize;
                        heavy[v] = u;
                    }
                }
            }
            
            return size;
        }
        
        void dfs2(int v, int h) {
            head[v] = h;
            pos[v] = timer++;
            
            if (heavy[v] != -1) {
                dfs2(heavy[v], h);
            }
            
            for (int u : adj[v]) {
                if (u != parent[v] && u != heavy[v]) {
                    dfs2(u, u);
                }
            }
        }
        
    public:
        HeavyLightLCA(int n) : n(n), timer(0) {
            adj.resize(n);
            parent.resize(n);
            depth.resize(n);
            heavy.assign(n, -1);
            head.resize(n);
            pos.resize(n);
        }
        
        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        void preprocess(int root = 0) {
            parent[root] = -1;
            depth[root] = 0;
            dfs1(root, -1);
            dfs2(root, root);
        }
        
        int lca(int a, int b) {
            while (head[a] != head[b]) {
                if (depth[head[a]] > depth[head[b]]) {
                    a = parent[head[a]];
                } else {
                    b = parent[head[b]];
                }
            }
            
            return (depth[a] < depth[b]) ? a : b;
        }
        
        int distance(int a, int b) {
            return depth[a] + depth[b] - 2 * depth[lca(a, b)];
        }
    };
}

// Example usage and testing
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, q;
    cin >> n >> q;
    
    // Example with Binary Lifting LCA
    LCA::BinaryLifting lca(n);
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--; // Convert to 0-indexed
        lca.addEdge(u, v);
    }
    
    lca.preprocess(0); // Root at node 0
    
    cout << "LCA queries:" << endl;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--; // Convert to 0-indexed
        
        int ancestor = lca.lca(a, b);
        int dist = lca.distance(a, b);
        
        cout << "LCA(" << a + 1 << ", " << b + 1 << ") = " << ancestor + 1 << endl;
        cout << "Distance = " << dist << endl;
    }
    
    // Example with different LCA implementations
    cout << "\nTesting different LCA implementations:" << endl;
    
    // Simple LCA
    LCA::SimpleLCA simpleLCA(5);
    simpleLCA.addEdge(0, 1);
    simpleLCA.addEdge(0, 2);
    simpleLCA.addEdge(1, 3);
    simpleLCA.addEdge(1, 4);
    simpleLCA.preprocess(0);
    
    cout << "Simple LCA(3, 4) = " << simpleLCA.lca(3, 4) << endl;
    cout << "Distance(3, 4) = " << simpleLCA.distance(3, 4) << endl;
    
    // Euler Tour LCA
    LCA::EulerTourLCA eulerLCA(5);
    eulerLCA.addEdge(0, 1);
    eulerLCA.addEdge(0, 2);
    eulerLCA.addEdge(1, 3);
    eulerLCA.addEdge(1, 4);
    eulerLCA.preprocess(0);
    
    cout << "Euler Tour LCA(3, 4) = " << eulerLCA.lca(3, 4) << endl;
    cout << "Distance(3, 4) = " << eulerLCA.distance(3, 4) << endl;
    
    return 0;
}

/*
Complexity Analysis:

1. Binary Lifting LCA:
   - Preprocessing: O(n log n)
   - Query: O(log n)
   - Space: O(n log n)
   - Best for: General purpose, supports k-th ancestor queries

2. Euler Tour + Sparse Table LCA:
   - Preprocessing: O(n log n)
   - Query: O(1)
   - Space: O(n log n)
   - Best for: Many queries, when O(1) query time is needed

3. Simple LCA:
   - Preprocessing: O(n)
   - Query: O(n)
   - Space: O(n)
   - Best for: Small trees, few queries, memory-constrained environments

4. Heavy-Light Decomposition LCA:
   - Preprocessing: O(n)
   - Query: O(log^2 n)
   - Space: O(n)
   - Best for: When you also need path queries/updates

Choose the appropriate implementation based on your constraints:
- For competitive programming with many queries: Binary Lifting or Euler Tour
- For simple cases or memory constraints: Simple LCA
- For path operations: Heavy-Light Decomposition
*/