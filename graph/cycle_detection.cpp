#include <bits/stdc++.h>
using namespace std;

// Cycle detection in graphs
class CycleDetection {
public:
    int n;
    vector<vector<int>> adj;
    vector<bool> visited;
    vector<int> color; // 0: white, 1: gray, 2: black
    
    CycleDetection(int vertices) {
        n = vertices;
        adj.resize(n);
        visited.resize(n, false);
        color.resize(n, 0);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // For undirected graph
    }
    
    void addDirectedEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    void reset() {
        fill(visited.begin(), visited.end(), false);
        fill(color.begin(), color.end(), 0);
    }
    
    // Detect cycle in undirected graph using DFS
    bool hasCycleUndirectedUtil(int v, int parent) {
        visited[v] = true;
        
        for(int u : adj[v]) {
            if(!visited[u]) {
                if(hasCycleUndirectedUtil(u, v)) return true;
            } else if(u != parent) {
                return true;
            }
        }
        return false;
    }
    
    bool hasCycleUndirected() {
        reset();
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                if(hasCycleUndirectedUtil(i, -1)) return true;
            }
        }
        return false;
    }
    
    // Detect cycle in directed graph using DFS (3-coloring)
    bool hasCycleDirectedUtil(int v) {
        color[v] = 1; // Gray
        
        for(int u : adj[v]) {
            if(color[u] == 1) return true; // Back edge found
            if(color[u] == 0 && hasCycleDirectedUtil(u)) return true;
        }
        
        color[v] = 2; // Black
        return false;
    }
    
    bool hasCycleDirected() {
        reset();
        
        for(int i = 0; i < n; i++) {
            if(color[i] == 0) {
                if(hasCycleDirectedUtil(i)) return true;
            }
        }
        return false;
    }
    
    // Find cycle in undirected graph
    vector<int> findCycleUndirected() {
        reset();
        vector<int> parent(n, -1);
        
        function<vector<int>(int, int)> dfs = [&](int v, int p) -> vector<int> {
            visited[v] = true;
            
            for(int u : adj[v]) {
                if(u == p) continue;
                
                if(visited[u]) {
                    // Found cycle, reconstruct it
                    vector<int> cycle;
                    cycle.push_back(u);
                    for(int curr = v; curr != u; curr = parent[curr]) {
                        cycle.push_back(curr);
                    }
                    cycle.push_back(u);
                    return cycle;
                }
                
                parent[u] = v;
                vector<int> result = dfs(u, v);
                if(!result.empty()) return result;
            }
            
            return {};
        };
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                vector<int> cycle = dfs(i, -1);
                if(!cycle.empty()) return cycle;
            }
        }
        
        return {};
    }
    
    // Find cycle in directed graph
    vector<int> findCycleDirected() {
        reset();
        vector<int> parent(n, -1);
        vector<int> cycle;
        
        function<bool(int)> dfs = [&](int v) -> bool {
            color[v] = 1;
            
            for(int u : adj[v]) {
                if(color[u] == 1) {
                    // Found back edge, reconstruct cycle
                    cycle.clear();
                    cycle.push_back(u);
                    for(int curr = v; curr != u; curr = parent[curr]) {
                        cycle.push_back(curr);
                    }
                    cycle.push_back(u);
                    reverse(cycle.begin(), cycle.end());
                    return true;
                }
                
                if(color[u] == 0) {
                    parent[u] = v;
                    if(dfs(u)) return true;
                }
            }
            
            color[v] = 2;
            return false;
        };
        
        for(int i = 0; i < n; i++) {
            if(color[i] == 0) {
                if(dfs(i)) return cycle;
            }
        }
        
        return {};
    }
    
    // Check if graph is bipartite (no odd cycles)
    bool isBipartite() {
        reset();
        vector<int> color(n, -1);
        
        function<bool(int, int)> dfs = [&](int v, int c) -> bool {
            color[v] = c;
            
            for(int u : adj[v]) {
                if(color[u] == -1) {
                    if(!dfs(u, 1 - c)) return false;
                } else if(color[u] == c) {
                    return false;
                }
            }
            return true;
        };
        
        for(int i = 0; i < n; i++) {
            if(color[i] == -1) {
                if(!dfs(i, 0)) return false;
            }
        }
        return true;
    }
    
    // Find all cycles in undirected graph (simple cycles)
    vector<vector<int>> findAllCyclesUndirected() {
        vector<vector<int>> cycles;
        reset();
        
        function<void(int, int, vector<int>&)> dfs = [&](int v, int start, vector<int>& path) {
            visited[v] = true;
            path.push_back(v);
            
            for(int u : adj[v]) {
                if(path.size() > 2 && u == start) {
                    // Found cycle
                    vector<int> cycle = path;
                    cycle.push_back(start);
                    cycles.push_back(cycle);
                } else if(!visited[u] && (path.empty() || u != path[path.size()-2])) {
                    dfs(u, start, path);
                }
            }
            
            path.pop_back();
            visited[v] = false;
        };
        
        for(int i = 0; i < n; i++) {
            vector<int> path;
            dfs(i, i, path);
            
            // Mark this vertex as processed to avoid duplicate cycles
            for(int j = 0; j < adj[i].size(); j++) {
                if(adj[i][j] > i) {
                    adj[adj[i][j]].erase(
                        find(adj[adj[i][j]].begin(), adj[adj[i][j]].end(), i)
                    );
                }
            }
        }
        
        return cycles;
    }
};

// Example usage:
/*
int main() {
    // Undirected graph
    CycleDetection g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 4);
    g1.addEdge(4, 1); // Creates cycle
    
    cout << "Undirected graph has cycle: " << (g1.hasCycleUndirected() ? "Yes" : "No") << endl;
    
    vector<int> cycle = g1.findCycleUndirected();
    if(!cycle.empty()) {
        cout << "Cycle found: ";
        for(int v : cycle) cout << v << " ";
        cout << endl;
    }
    
    // Directed graph
    CycleDetection g2(4);
    g2.addDirectedEdge(0, 1);
    g2.addDirectedEdge(1, 2);
    g2.addDirectedEdge(2, 3);
    g2.addDirectedEdge(3, 1); // Creates cycle
    
    cout << "Directed graph has cycle: " << (g2.hasCycleDirected() ? "Yes" : "No") << endl;
    
    return 0;
}
*/