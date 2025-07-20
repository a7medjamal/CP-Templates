#include <bits/stdc++.h>
using namespace std;

// Topological Sort algorithms
class TopologicalSort {
public:
    int n;
    vector<vector<int>> adj;
    vector<bool> visited;
    stack<int> topoStack;
    
    TopologicalSort(int vertices) {
        n = vertices;
        adj.resize(n);
        visited.resize(n, false);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    void reset() {
        fill(visited.begin(), visited.end(), false);
        while(!topoStack.empty()) topoStack.pop();
    }
    
    // DFS-based topological sort
    void dfs(int v) {
        visited[v] = true;
        
        for(int u : adj[v]) {
            if(!visited[u]) {
                dfs(u);
            }
        }
        
        topoStack.push(v);
    }
    
    vector<int> topologicalSort() {
        reset();
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                dfs(i);
            }
        }
        
        vector<int> result;
        while(!topoStack.empty()) {
            result.push_back(topoStack.top());
            topoStack.pop();
        }
        
        return result;
    }
    
    // Kahn's algorithm for topological sort
    vector<int> kahnTopologicalSort() {
        vector<int> indegree(n, 0);
        
        // Calculate indegrees
        for(int u = 0; u < n; u++) {
            for(int v : adj[u]) {
                indegree[v]++;
            }
        }
        
        queue<int> q;
        for(int i = 0; i < n; i++) {
            if(indegree[i] == 0) {
                q.push(i);
            }
        }
        
        vector<int> result;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(u);
            
            for(int v : adj[u]) {
                indegree[v]--;
                if(indegree[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        return result.size() == n ? result : vector<int>(); // Empty if cycle exists
    }
    
    // Check if topological sort is possible (DAG check)
    bool isDAG() {
        return kahnTopologicalSort().size() == n;
    }
    
    // Find lexicographically smallest topological sort
    vector<int> lexicographicallySmallestTopoSort() {
        vector<int> indegree(n, 0);
        
        for(int u = 0; u < n; u++) {
            for(int v : adj[u]) {
                indegree[v]++;
            }
        }
        
        priority_queue<int, vector<int>, greater<int>> pq;
        for(int i = 0; i < n; i++) {
            if(indegree[i] == 0) {
                pq.push(i);
            }
        }
        
        vector<int> result;
        while(!pq.empty()) {
            int u = pq.top();
            pq.pop();
            result.push_back(u);
            
            for(int v : adj[u]) {
                indegree[v]--;
                if(indegree[v] == 0) {
                    pq.push(v);
                }
            }
        }
        
        return result.size() == n ? result : vector<int>();
    }
    
    // Count number of topological sorts
    int countTopologicalSorts() {
        vector<int> indegree(n, 0);
        
        for(int u = 0; u < n; u++) {
            for(int v : adj[u]) {
                indegree[v]++;
            }
        }
        
        function<int(vector<int>&, vector<bool>&)> backtrack = [&](vector<int>& indeg, vector<bool>& used) -> int {
            vector<int> available;
            for(int i = 0; i < n; i++) {
                if(!used[i] && indeg[i] == 0) {
                    available.push_back(i);
                }
            }
            
            if(available.empty()) {
                // Check if all vertices are used
                for(int i = 0; i < n; i++) {
                    if(!used[i]) return 0;
                }
                return 1;
            }
            
            int count = 0;
            for(int u : available) {
                used[u] = true;
                
                // Decrease indegree of neighbors
                for(int v : adj[u]) {
                    indeg[v]--;
                }
                
                count += backtrack(indeg, used);
                
                // Backtrack
                for(int v : adj[u]) {
                    indeg[v]++;
                }
                used[u] = false;
            }
            
            return count;
        };
        
        vector<bool> used(n, false);
        return backtrack(indegree, used);
    }
    
    // Find all possible topological sorts
    vector<vector<int>> allTopologicalSorts() {
        vector<vector<int>> result;
        vector<int> indegree(n, 0);
        
        for(int u = 0; u < n; u++) {
            for(int v : adj[u]) {
                indegree[v]++;
            }
        }
        
        function<void(vector<int>&, vector<bool>&, vector<int>&)> backtrack = 
            [&](vector<int>& indeg, vector<bool>& used, vector<int>& current) {
            if(current.size() == n) {
                result.push_back(current);
                return;
            }
            
            for(int i = 0; i < n; i++) {
                if(!used[i] && indeg[i] == 0) {
                    used[i] = true;
                    current.push_back(i);
                    
                    // Decrease indegree of neighbors
                    for(int v : adj[i]) {
                        indeg[v]--;
                    }
                    
                    backtrack(indeg, used, current);
                    
                    // Backtrack
                    for(int v : adj[i]) {
                        indeg[v]++;
                    }
                    current.pop_back();
                    used[i] = false;
                }
            }
        };
        
        vector<bool> used(n, false);
        vector<int> current;
        backtrack(indegree, used, current);
        
        return result;
    }
};

// Example usage:
/*
int main() {
    TopologicalSort ts(6);
    ts.addEdge(5, 2);
    ts.addEdge(5, 0);
    ts.addEdge(4, 0);
    ts.addEdge(4, 1);
    ts.addEdge(2, 3);
    ts.addEdge(3, 1);
    
    vector<int> topo = ts.topologicalSort();
    cout << "DFS Topological Sort: ";
    for(int v : topo) cout << v << " ";
    cout << endl;
    
    vector<int> kahn = ts.kahnTopologicalSort();
    cout << "Kahn's Topological Sort: ";
    for(int v : kahn) cout << v << " ";
    cout << endl;
    
    cout << "Is DAG: " << (ts.isDAG() ? "Yes" : "No") << endl;
    cout << "Number of topological sorts: " << ts.countTopologicalSorts() << endl;
    
    return 0;
}
*/