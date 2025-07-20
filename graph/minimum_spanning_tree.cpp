#include <bits/stdc++.h>
using namespace std;

// Union-Find data structure for Kruskal's algorithm
class UnionFind {
public:
    vector<int> parent, rank;
    int components;
    
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        components = n;
        for(int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if(parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if(px == py) return false;
        
        if(rank[px] < rank[py]) {
            parent[px] = py;
        } else if(rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
        components--;
        return true;
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

// Edge structure for MST algorithms
struct Edge {
    int u, v, weight;
    
    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Minimum Spanning Tree algorithms
class MST {
public:
    int n;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj; // adjacency list for Prim's
    
    MST(int vertices) {
        n = vertices;
        adj.resize(n);
    }
    
    void addEdge(int u, int v, int weight) {
        edges.push_back(Edge(u, v, weight));
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }
    
    // Kruskal's algorithm
    pair<long long, vector<Edge>> kruskal() {
        vector<Edge> mst;
        long long totalWeight = 0;
        
        sort(edges.begin(), edges.end());
        UnionFind uf(n);
        
        for(const Edge& e : edges) {
            if(uf.unite(e.u, e.v)) {
                mst.push_back(e);
                totalWeight += e.weight;
                if(mst.size() == n - 1) break;
            }
        }
        
        return {totalWeight, mst};
    }
    
    // Prim's algorithm
    pair<long long, vector<Edge>> prim(int start = 0) {
        vector<Edge> mst;
        vector<bool> inMST(n, false);
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
        long long totalWeight = 0;
        
        inMST[start] = true;
        for(auto& edge : adj[start]) {
            pq.push({edge.second, {start, edge.first}});
        }
        
        while(!pq.empty() && mst.size() < n - 1) {
            auto current = pq.top();
            pq.pop();
            
            int weight = current.first;
            int u = current.second.first;
            int v = current.second.second;
            
            if(inMST[v]) continue;
            
            inMST[v] = true;
            mst.push_back(Edge(u, v, weight));
            totalWeight += weight;
            
            for(auto& edge : adj[v]) {
                if(!inMST[edge.first]) {
                    pq.push({edge.second, {v, edge.first}});
                }
            }
        }
        
        return {totalWeight, mst};
    }
    
    // Borůvka's algorithm
    pair<long long, vector<Edge>> boruvka() {
        vector<Edge> mst;
        long long totalWeight = 0;
        UnionFind uf(n);
        
        while(uf.components > 1) {
            vector<int> cheapest(n, -1);
            
            // Find cheapest edge for each component
            for(int i = 0; i < (int)edges.size(); i++) {
                int u = edges[i].u, v = edges[i].v, w = edges[i].weight;
                int setU = uf.find(u), setV = uf.find(v);
                
                if(setU != setV) {
                    if(cheapest[setU] == -1 || edges[cheapest[setU]].weight > w) {
                        cheapest[setU] = i;
                    }
                    if(cheapest[setV] == -1 || edges[cheapest[setV]].weight > w) {
                        cheapest[setV] = i;
                    }
                }
            }
            
            // Add cheapest edges to MST
            for(int i = 0; i < n; i++) {
                if(cheapest[i] != -1) {
                    Edge& e = edges[cheapest[i]];
                    if(uf.unite(e.u, e.v)) {
                        mst.push_back(e);
                        totalWeight += e.weight;
                    }
                }
            }
        }
        
        return {totalWeight, mst};
    }
    
    // Check if graph is connected (MST exists)
    bool isConnected() {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(0);
        visited[0] = true;
        int count = 1;
        
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            
            for(auto& edge : adj[u]) {
                int v = edge.first;
                if(!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                    count++;
                }
            }
        }
        
        return count == n;
    }
    
    // Find second minimum spanning tree
    pair<long long, vector<Edge>> secondMST() {
        auto [mstWeight, mstEdges] = kruskal();
        if(mstEdges.size() != n - 1) return {-1, {}}; // No MST exists
        
        long long secondMSTWeight = LLONG_MAX;
        vector<Edge> secondMSTEdges;
        
        // Try removing each edge from MST and find next best MST
        for(int i = 0; i < (int)mstEdges.size(); i++) {
            vector<Edge> tempEdges;
            for(int j = 0; j < (int)edges.size(); j++) {
                if(!(edges[j].u == mstEdges[i].u && edges[j].v == mstEdges[i].v && edges[j].weight == mstEdges[i].weight) &&
                   !(edges[j].u == mstEdges[i].v && edges[j].v == mstEdges[i].u && edges[j].weight == mstEdges[i].weight)) {
                    tempEdges.push_back(edges[j]);
                }
            }
            
            sort(tempEdges.begin(), tempEdges.end());
            UnionFind uf(n);
            vector<Edge> currentMST;
            long long currentWeight = 0;
            
            for(const Edge& e : tempEdges) {
                if(uf.unite(e.u, e.v)) {
                    currentMST.push_back(e);
                    currentWeight += e.weight;
                    if(currentMST.size() == n - 1) break;
                }
            }
            
            if(currentMST.size() == n - 1 && currentWeight < secondMSTWeight) {
                secondMSTWeight = currentWeight;
                secondMSTEdges = currentMST;
            }
        }
        
        return secondMSTWeight == LLONG_MAX ? make_pair(-1LL, vector<Edge>()) : make_pair(secondMSTWeight, secondMSTEdges);
    }
    
    // Find all MSTs (when multiple MSTs exist)
    vector<pair<long long, vector<Edge>>> allMSTs() {
        vector<pair<long long, vector<Edge>>> result;
        sort(edges.begin(), edges.end());
        
        function<void(int, UnionFind&, vector<Edge>&, long long)> backtrack = 
            [&](int edgeIdx, UnionFind& uf, vector<Edge>& currentMST, long long currentWeight) {
            if(currentMST.size() == n - 1) {
                result.push_back({currentWeight, currentMST});
                return;
            }
            
            if(edgeIdx >= edges.size()) return;
            
            // Try including current edge
            Edge& e = edges[edgeIdx];
            if(!uf.connected(e.u, e.v)) {
                UnionFind newUF = uf;
                newUF.unite(e.u, e.v);
                currentMST.push_back(e);
                backtrack(edgeIdx + 1, newUF, currentMST, currentWeight + e.weight);
                currentMST.pop_back();
            }
            
            // Try skipping current edge
            backtrack(edgeIdx + 1, uf, currentMST, currentWeight);
        };
        
        UnionFind uf(n);
        vector<Edge> currentMST;
        backtrack(0, uf, currentMST, 0);
        
        // Filter to keep only minimum weight MSTs
        if(!result.empty()) {
            long long minWeight = result[0].first;
            for(auto& mst : result) {
                minWeight = min(minWeight, mst.first);
            }
            
            vector<pair<long long, vector<Edge>>> filteredResult;
            for(auto& mst : result) {
                if(mst.first == minWeight) {
                    filteredResult.push_back(mst);
                }
            }
            result = filteredResult;
        }
        
        return result;
    }
    
    // Get MST using different algorithms and compare
    void compareMSTAlgorithms() {
        auto [kruskalWeight, kruskalMST] = kruskal();
        auto [primWeight, primMST] = prim();
        auto [boruvkaWeight, boruvkaMST] = boruvka();
        
        cout << "Kruskal's MST weight: " << kruskalWeight << endl;
        cout << "Prim's MST weight: " << primWeight << endl;
        cout << "Borůvka's MST weight: " << boruvkaWeight << endl;
        
        cout << "All algorithms agree: " << 
                (kruskalWeight == primWeight && primWeight == boruvkaWeight ? "Yes" : "No") << endl;
    }
};

// Example usage:
/*
int main() {
    MST mst(4);
    mst.addEdge(0, 1, 10);
    mst.addEdge(0, 2, 6);
    mst.addEdge(0, 3, 5);
    mst.addEdge(1, 3, 15);
    mst.addEdge(2, 3, 4);
    
    auto [weight, edges] = mst.kruskal();
    cout << "MST weight: " << weight << endl;
    cout << "MST edges: ";
    for(const Edge& e : edges) {
        cout << "(" << e.u << ", " << e.v << ", " << e.weight << ") ";
    }
    cout << endl;
    
    mst.compareMSTAlgorithms();
    
    return 0;
}
*/