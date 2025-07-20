#include <bits/stdc++.h>
using namespace std;

// Disjoint Set Union (Union-Find) with path compression and union by rank
class DSU {
public:
    vector<int> parent, rank, size;
    int components;
    
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        size.resize(n, 1);
        components = n;
        for(int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if(parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if(px == py) return false;
        
        // Union by rank
        if(rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        size[px] += size[py];
        if(rank[px] == rank[py]) rank[px]++;
        
        components--;
        return true;
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
    
    int getSize(int x) {
        return size[find(x)];
    }
    
    int getComponents() {
        return components;
    }
};

// Example usage:
/*
int main() {
    int n = 5;
    DSU dsu(n);
    
    dsu.unite(0, 1);
    dsu.unite(2, 3);
    
    cout << dsu.connected(0, 1) << endl; // 1
    cout << dsu.connected(0, 2) << endl; // 0
    cout << dsu.getSize(0) << endl;      // 2
    cout << dsu.getComponents() << endl; // 3
    
    return 0;
}
*/