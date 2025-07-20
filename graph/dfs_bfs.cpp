#include <bits/stdc++.h>
using namespace std;

// Graph representation for DFS and BFS traversals
class Graph {
public:
    int n;
    vector<vector<int>> adj;
    vector<bool> visited;
    
    Graph(int vertices) {
        n = vertices;
        adj.resize(n);
        visited.resize(n, false);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // For undirected graph
    }
    
    void addDirectedEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    void resetVisited() {
        fill(visited.begin(), visited.end(), false);
    }
    
    // Depth First Search
    void dfs(int v) {
        visited[v] = true;
        cout << v << " ";
        
        for(int u : adj[v]) {
            if(!visited[u]) {
                dfs(u);
            }
        }
    }
    
    // DFS with custom processing function
    void dfs(int v, function<void(int)> process) {
        visited[v] = true;
        process(v);
        
        for(int u : adj[v]) {
            if(!visited[u]) {
                dfs(u, process);
            }
        }
    }
    
    // Breadth First Search
    void bfs(int start) {
        resetVisited();
        queue<int> q;
        visited[start] = true;
        q.push(start);
        
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";
            
            for(int u : adj[v]) {
                if(!visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }
            }
        }
    }
    
    // BFS with custom processing function
    void bfs(int start, function<void(int)> process) {
        resetVisited();
        queue<int> q;
        visited[start] = true;
        q.push(start);
        
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            process(v);
            
            for(int u : adj[v]) {
                if(!visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }
            }
        }
    }
    
    // BFS to find shortest path in unweighted graph
    vector<int> bfsShortestPath(int start, int end) {
        resetVisited();
        queue<int> q;
        vector<int> parent(n, -1);
        visited[start] = true;
        q.push(start);
        
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            
            if(v == end) break;
            
            for(int u : adj[v]) {
                if(!visited[u]) {
                    visited[u] = true;
                    parent[u] = v;
                    q.push(u);
                }
            }
        }
        
        vector<int> path;
        if(!visited[end]) return path; // No path exists
        
        for(int v = end; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path;
    }
    
    // Check if graph is connected
    bool isConnected() {
        resetVisited();
        dfs(0);
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) return false;
        }
        return true;
    }
    
    // Count connected components
    int countComponents() {
        resetVisited();
        int components = 0;
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                dfs(i);
                components++;
            }
        }
        return components;
    }
    
    // Get all connected components
    vector<vector<int>> getComponents() {
        resetVisited();
        vector<vector<int>> components;
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                vector<int> component;
                dfs(i, [&](int v) { component.push_back(v); });
                components.push_back(component);
            }
        }
        return components;
    }
};

// Example usage:
/*
int main() {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    
    cout << "DFS from vertex 0: ";
    g.dfs(0);
    cout << endl;
    
    cout << "BFS from vertex 0: ";
    g.bfs(0);
    cout << endl;
    
    cout << "Connected components: " << g.countComponents() << endl;
    
    return 0;
}
*/