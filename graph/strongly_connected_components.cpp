#include <bits/stdc++.h>
using namespace std;

// Strongly Connected Components using Tarjan's algorithm
class SCC {
public:
    int n, timer, sccCount;
    vector<vector<int>> adj;
    vector<int> disc, low, stackMember;
    vector<bool> visited;
    stack<int> st;
    vector<vector<int>> sccs;
    vector<int> sccId;
    
    SCC(int vertices) {
        n = vertices;
        timer = 0;
        sccCount = 0;
        adj.resize(n);
        disc.resize(n, -1);
        low.resize(n, -1);
        stackMember.resize(n, false);
        visited.resize(n, false);
        sccId.resize(n, -1);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    void reset() {
        timer = 0;
        sccCount = 0;
        fill(disc.begin(), disc.end(), -1);
        fill(low.begin(), low.end(), -1);
        fill(stackMember.begin(), stackMember.end(), false);
        fill(visited.begin(), visited.end(), false);
        fill(sccId.begin(), sccId.end(), -1);
        while(!st.empty()) st.pop();
        sccs.clear();
    }
    
    void tarjanDFS(int u) {
        disc[u] = low[u] = timer++;
        st.push(u);
        stackMember[u] = true;
        
        for(int v : adj[u]) {
            if(disc[v] == -1) {
                tarjanDFS(v);
                low[u] = min(low[u], low[v]);
            }
            else if(stackMember[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }
        
        if(low[u] == disc[u]) {
            vector<int> component;
            int w;
            do {
                w = st.top();
                st.pop();
                stackMember[w] = false;
                component.push_back(w);
                sccId[w] = sccCount;
            } while(w != u);
            
            sccs.push_back(component);
            sccCount++;
        }
    }
    
    vector<vector<int>> findSCCs() {
        reset();
        
        for(int i = 0; i < n; i++) {
            if(disc[i] == -1) {
                tarjanDFS(i);
            }
        }
        
        return sccs;
    }
    
    // Check if graph is strongly connected
    bool isStronglyConnected() {
        findSCCs();
        return sccCount == 1;
    }
    
    // Get condensation graph (DAG of SCCs)
    vector<vector<int>> getCondensationGraph() {
        findSCCs();
        vector<vector<int>> condensed(sccCount);
        vector<set<int>> edges(sccCount);
        
        for(int u = 0; u < n; u++) {
            for(int v : adj[u]) {
                if(sccId[u] != sccId[v]) {
                    edges[sccId[u]].insert(sccId[v]);
                }
            }
        }
        
        for(int i = 0; i < sccCount; i++) {
            for(int j : edges[i]) {
                condensed[i].push_back(j);
            }
        }
        
        return condensed;
    }
    
    // Find bridges in the graph
    vector<pair<int, int>> findBridges() {
        vector<pair<int, int>> bridges;
        vector<int> disc(n, -1), low(n, -1), parent(n, -1);
        vector<bool> visited(n, false);
        int timer = 0;
        
        function<void(int)> bridgeUtil = [&](int u) {
            visited[u] = true;
            disc[u] = low[u] = timer++;
            
            for(int v : adj[u]) {
                if(!visited[v]) {
                    parent[v] = u;
                    bridgeUtil(v);
                    
                    low[u] = min(low[u], low[v]);
                    
                    if(low[v] > disc[u]) {
                        bridges.push_back({min(u, v), max(u, v)});
                    }
                }
                else if(v != parent[u]) {
                    low[u] = min(low[u], disc[v]);
                }
            }
        };
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                bridgeUtil(i);
            }
        }
        
        return bridges;
    }
    
    // Find articulation points
    vector<int> findArticulationPoints() {
        vector<int> artPoints;
        vector<int> disc(n, -1), low(n, -1), parent(n, -1);
        vector<bool> visited(n, false), ap(n, false);
        int timer = 0;
        
        function<void(int)> apUtil = [&](int u) {
            int children = 0;
            visited[u] = true;
            disc[u] = low[u] = timer++;
            
            for(int v : adj[u]) {
                if(!visited[v]) {
                    children++;
                    parent[v] = u;
                    apUtil(v);
                    
                    low[u] = min(low[u], low[v]);
                    
                    if(parent[u] == -1 && children > 1) {
                        ap[u] = true;
                    }
                    
                    if(parent[u] != -1 && low[v] >= disc[u]) {
                        ap[u] = true;
                    }
                }
                else if(v != parent[u]) {
                    low[u] = min(low[u], disc[v]);
                }
            }
        };
        
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                apUtil(i);
            }
        }
        
        for(int i = 0; i < n; i++) {
            if(ap[i]) {
                artPoints.push_back(i);
            }
        }
        
        return artPoints;
    }
    
    // Check if removing a vertex disconnects the graph
    bool isArticulationPoint(int vertex) {
        vector<int> ap = findArticulationPoints();
        return find(ap.begin(), ap.end(), vertex) != ap.end();
    }
    
    // Get SCC containing a specific vertex
    vector<int> getSCCContaining(int vertex) {
        if(sccs.empty()) findSCCs();
        if(sccId[vertex] == -1) return {};
        return sccs[sccId[vertex]];
    }
    
    // Check if two vertices are in the same SCC
    bool inSameSCC(int u, int v) {
        if(sccs.empty()) findSCCs();
        return sccId[u] != -1 && sccId[u] == sccId[v];
    }
};

// Kosaraju's algorithm for SCC (alternative implementation)
class KosarajuSCC {
public:
    int n;
    vector<vector<int>> adj, radj;
    vector<bool> visited;
    vector<int> order, component;
    vector<vector<int>> sccs;
    
    KosarajuSCC(int vertices) {
        n = vertices;
        adj.resize(n);
        radj.resize(n);
        visited.resize(n);
        component.resize(n, -1);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }
    
    void dfs1(int v) {
        visited[v] = true;
        for(int u : adj[v]) {
            if(!visited[u]) {
                dfs1(u);
            }
        }
        order.push_back(v);
    }
    
    void dfs2(int v, int comp) {
        component[v] = comp;
        for(int u : radj[v]) {
            if(component[u] == -1) {
                dfs2(u, comp);
            }
        }
    }
    
    vector<vector<int>> findSCCs() {
        // Reset
        fill(visited.begin(), visited.end(), false);
        fill(component.begin(), component.end(), -1);
        order.clear();
        sccs.clear();
        
        // First DFS to get finishing times
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                dfs1(i);
            }
        }
        
        // Second DFS on transpose graph
        int comp = 0;
        reverse(order.begin(), order.end());
        for(int v : order) {
            if(component[v] == -1) {
                dfs2(v, comp++);
            }
        }
        
        // Group vertices by component
        sccs.resize(comp);
        for(int i = 0; i < n; i++) {
            sccs[component[i]].push_back(i);
        }
        
        return sccs;
    }
};

// Example usage:
/*
int main() {
    SCC scc(5);
    scc.addEdge(1, 0);
    scc.addEdge(0, 2);
    scc.addEdge(2, 1);
    scc.addEdge(0, 3);
    scc.addEdge(3, 4);
    
    vector<vector<int>> components = scc.findSCCs();
    cout << "Strongly Connected Components:" << endl;
    for(int i = 0; i < (int)components.size(); i++) {
        cout << "SCC " << i << ": ";
        for(int v : components[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
    
    cout << "Is strongly connected: " << (scc.isStronglyConnected() ? "Yes" : "No") << endl;
    
    vector<pair<int, int>> bridges = scc.findBridges();
    cout << "Bridges: ";
    for(auto bridge : bridges) {
        cout << "(" << bridge.first << ", " << bridge.second << ") ";
    }
    cout << endl;
    
    return 0;
}
*/