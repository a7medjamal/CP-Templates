#include <bits/stdc++.h>
using namespace std;

// Centroid Decomposition for Tree Divide-and-Conquer
class CentroidDecomposition {
public:
    int n;
    vector<vector<int>> adj;
    vector<bool> removed;
    vector<int> subtree_size;
    
    CentroidDecomposition(int n) : n(n) {
        adj.resize(n);
        removed.resize(n, false);
        subtree_size.resize(n);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    int getSubtreeSize(int v, int parent = -1) {
        subtree_size[v] = 1;
        for (int u : adj[v]) {
            if (u != parent && !removed[u]) {
                subtree_size[v] += getSubtreeSize(u, v);
            }
        }
        return subtree_size[v];
    }
    
    int getCentroid(int v, int tree_size, int parent = -1) {
        for (int u : adj[v]) {
            if (u != parent && !removed[u] && subtree_size[u] > tree_size / 2) {
                return getCentroid(u, tree_size, v);
            }
        }
        return v;
    }
    
    void decompose(int v = 0) {
        int tree_size = getSubtreeSize(v);
        int centroid = getCentroid(v, tree_size);
        
        removed[centroid] = true;
        
        // Process centroid here
        // Add your centroid-specific logic
        
        for (int u : adj[centroid]) {
            if (!removed[u]) {
                decompose(u);
            }
        }
    }
    
    // Example: Count paths of length k
    vector<int> countPaths(int k) {
        vector<int> result(n, 0);
        // Implementation depends on specific problem
        return result;
    }
};

// Example usage:
/*
int main() {
    CentroidDecomposition cd(6);
    cd.addEdge(0, 1);
    cd.addEdge(0, 2);
    cd.addEdge(1, 3);
    cd.addEdge(1, 4);
    cd.addEdge(2, 5);
    cd.decompose();
    
    return 0;
}
*/