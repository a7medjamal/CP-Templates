#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

// Weighted Graph for shortest path algorithms
class WeightedGraph {
public:
    int n;
    vector<vector<pair<int, int>>> adj; // {neighbor, weight}
    
    WeightedGraph(int vertices) {
        n = vertices;
        adj.resize(n);
    }
    
    void addEdge(int u, int v, int weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight}); // For undirected graph
    }
    
    void addDirectedEdge(int u, int v, int weight) {
        adj[u].push_back({v, weight});
    }
    
    // Dijkstra's shortest path algorithm
    vector<int> dijkstra(int start) {
        vector<int> dist(n, INF);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[start] = 0;
        pq.push({0, start});
        
        while(!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            
            if(d > dist[u]) continue;
            
            for(auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                if(dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return dist;
    }
    
    // Dijkstra with path reconstruction
    pair<vector<int>, vector<int>> dijkstraWithPath(int start) {
        vector<int> dist(n, INF);
        vector<int> parent(n, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[start] = 0;
        pq.push({0, start});
        
        while(!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            
            if(d > dist[u]) continue;
            
            for(auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                if(dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return {dist, parent};
    }
    
    // Get path from start to end using parent array
    vector<int> getPath(const vector<int>& parent, int start, int end) {
        vector<int> path;
        for(int v = end; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path[0] == start ? path : vector<int>();
    }
    
    // Bellman-Ford algorithm (handles negative weights)
    vector<int> bellmanFord(int start) {
        vector<int> dist(n, INF);
        dist[start] = 0;
        
        // Relax edges n-1 times
        for(int i = 0; i < n - 1; i++) {
            for(int u = 0; u < n; u++) {
                if(dist[u] == INF) continue;
                
                for(auto& edge : adj[u]) {
                    int v = edge.first;
                    int weight = edge.second;
                    
                    if(dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                    }
                }
            }
        }
        
        // Check for negative cycles
        for(int u = 0; u < n; u++) {
            if(dist[u] == INF) continue;
            
            for(auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                if(dist[u] + weight < dist[v]) {
                    // Negative cycle detected
                    return {};
                }
            }
        }
        
        return dist;
    }
    
    // SPFA (Shortest Path Faster Algorithm)
    vector<int> spfa(int start) {
        vector<int> dist(n, INF);
        vector<bool> inQueue(n, false);
        vector<int> count(n, 0);
        queue<int> q;
        
        dist[start] = 0;
        q.push(start);
        inQueue[start] = true;
        
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            inQueue[u] = false;
            
            for(auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                if(dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    
                    if(!inQueue[v]) {
                        q.push(v);
                        inQueue[v] = true;
                        count[v]++;
                        
                        // Negative cycle detection
                        if(count[v] >= n) {
                            return {}; // Negative cycle
                        }
                    }
                }
            }
        }
        
        return dist;
    }
};

// Floyd-Warshall algorithm for all-pairs shortest paths
class FloydWarshall {
public:
    int n;
    vector<vector<int>> dist;
    vector<vector<int>> next;
    
    FloydWarshall(int vertices) {
        n = vertices;
        dist.assign(n, vector<int>(n, INF));
        next.assign(n, vector<int>(n, -1));
        
        for(int i = 0; i < n; i++) {
            dist[i][i] = 0;
        }
    }
    
    void addEdge(int u, int v, int weight) {
        dist[u][v] = weight;
        next[u][v] = v;
    }
    
    void floydWarshall() {
        for(int k = 0; k < n; k++) {
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < n; j++) {
                    if(dist[i][k] != INF && dist[k][j] != INF) {
                        if(dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                        }
                    }
                }
            }
        }
    }
    
    vector<int> getPath(int start, int end) {
        if(next[start][end] == -1) return {};
        
        vector<int> path = {start};
        while(start != end) {
            start = next[start][end];
            path.push_back(start);
        }
        return path;
    }
    
    bool hasNegativeCycle() {
        for(int i = 0; i < n; i++) {
            if(dist[i][i] < 0) return true;
        }
        return false;
    }
};

// Example usage:
/*
int main() {
    WeightedGraph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 4, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 4, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 2, 6);
    g.addEdge(3, 0, 7);
    g.addEdge(4, 1, 3);
    g.addEdge(4, 2, 9);
    g.addEdge(4, 3, 2);
    
    vector<int> dist = g.dijkstra(0);
    cout << "Shortest distances from vertex 0:" << endl;
    for(int i = 0; i < (int)dist.size(); i++) {
        cout << "To " << i << ": " << dist[i] << endl;
    }
    
    return 0;
}
*/