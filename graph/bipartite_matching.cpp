#include <bits/stdc++.h>
using namespace std;

// Bipartite Matching Algorithms
namespace BipartiteMatching {
    
    // Kuhn's Algorithm for Maximum Bipartite Matching
    class KuhnMatching {
    public:
        vector<vector<int>> graph;
        vector<int> match;
        vector<bool> used;
        int n, m;
        
        KuhnMatching(int n, int m) : n(n), m(m) {
            graph.resize(n);
            match.resize(m, -1);
            used.resize(n);
        }
        
        void addEdge(int u, int v) {
            graph[u].push_back(v);
        }
        
        bool dfs(int v) {
            if (used[v]) return false;
            used[v] = true;
            
            for (int to : graph[v]) {
                if (match[to] == -1 || dfs(match[to])) {
                    match[to] = v;
                    return true;
                }
            }
            
            return false;
        }
        
        int maxMatching() {
            int result = 0;
            
            for (int v = 0; v < n; ++v) {
                fill(used.begin(), used.end(), false);
                if (dfs(v)) {
                    result++;
                }
            }
            
            return result;
        }
        
        vector<pair<int, int>> getMatching() {
            vector<pair<int, int>> matching;
            for (int i = 0; i < m; i++) {
                if (match[i] != -1) {
                    matching.push_back({match[i], i});
                }
            }
            return matching;
        }
        
        // Find minimum vertex cover
        vector<int> minVertexCover() {
            maxMatching(); // Ensure matching is computed
            
            fill(used.begin(), used.end(), false);
            
            // Find unmatched vertices in left part
            for (int v = 0; v < n; v++) {
                bool matched = false;
                for (int i = 0; i < m; i++) {
                    if (match[i] == v) {
                        matched = true;
                        break;
                    }
                }
                if (!matched) {
                    dfs(v);
                }
            }
            
            vector<int> cover;
            
            // Add unvisited vertices from left part
            for (int v = 0; v < n; v++) {
                if (!used[v]) {
                    cover.push_back(v);
                }
            }
            
            // Add visited vertices from right part
            for (int i = 0; i < m; i++) {
                if (match[i] != -1 && used[match[i]]) {
                    cover.push_back(n + i);
                }
            }
            
            return cover;
        }
        
        // Find maximum independent set
        vector<int> maxIndependentSet() {
            auto cover = minVertexCover();
            vector<bool> inCover(n + m, false);
            
            for (int v : cover) {
                inCover[v] = true;
            }
            
            vector<int> independentSet;
            for (int i = 0; i < n + m; i++) {
                if (!inCover[i]) {
                    independentSet.push_back(i);
                }
            }
            
            return independentSet;
        }
    };
    
    // Hopcroft-Karp Algorithm for Maximum Bipartite Matching
    class HopcroftKarp {
    public:
        vector<vector<int>> graph;
        vector<int> pairU, pairV, dist;
        int n, m, NIL;
        
        HopcroftKarp(int n, int m) : n(n), m(m) {
            NIL = 0;
            graph.resize(n + 1);
            pairU.resize(n + 1);
            pairV.resize(m + 1);
            dist.resize(n + 1);
        }
        
        void addEdge(int u, int v) {
            graph[u + 1].push_back(v + 1);
        }
        
        bool bfs() {
            queue<int> q;
            
            for (int u = 1; u <= n; u++) {
                if (pairU[u] == NIL) {
                    dist[u] = 0;
                    q.push(u);
                } else {
                    dist[u] = INT_MAX;
                }
            }
            
            dist[NIL] = INT_MAX;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                if (dist[u] < dist[NIL]) {
                    for (int v : graph[u]) {
                        if (dist[pairV[v]] == INT_MAX) {
                            dist[pairV[v]] = dist[u] + 1;
                            q.push(pairV[v]);
                        }
                    }
                }
            }
            
            return dist[NIL] != INT_MAX;
        }
        
        bool dfs(int u) {
            if (u != NIL) {
                for (int v : graph[u]) {
                    if (dist[pairV[v]] == dist[u] + 1) {
                        if (dfs(pairV[v])) {
                            pairV[v] = u;
                            pairU[u] = v;
                            return true;
                        }
                    }
                }
                
                dist[u] = INT_MAX;
                return false;
            }
            
            return true;
        }
        
        int maxMatching() {
            fill(pairU.begin(), pairU.end(), NIL);
            fill(pairV.begin(), pairV.end(), NIL);
            
            int matching = 0;
            
            while (bfs()) {
                for (int u = 1; u <= n; u++) {
                    if (pairU[u] == NIL && dfs(u)) {
                        matching++;
                    }
                }
            }
            
            return matching;
        }
        
        vector<pair<int, int>> getMatching() {
            vector<pair<int, int>> matching;
            for (int u = 1; u <= n; u++) {
                if (pairU[u] != NIL) {
                    matching.push_back({u - 1, pairU[u] - 1});
                }
            }
            return matching;
        }
    };
    
    // Ford-Fulkerson based Bipartite Matching
    class FlowMatching {
    public:
        vector<vector<int>> capacity;
        vector<vector<int>> adj;
        int n, m, source, sink;
        
        FlowMatching(int n, int m) : n(n), m(m) {
            int total = n + m + 2;
            source = total - 2;
            sink = total - 1;
            
            capacity.assign(total, vector<int>(total, 0));
            adj.resize(total);
            
            // Connect source to left vertices
            for (int i = 0; i < n; i++) {
                adj[source].push_back(i);
                adj[i].push_back(source);
                capacity[source][i] = 1;
            }
            
            // Connect right vertices to sink
            for (int i = 0; i < m; i++) {
                adj[n + i].push_back(sink);
                adj[sink].push_back(n + i);
                capacity[n + i][sink] = 1;
            }
        }
        
        void addEdge(int u, int v) {
            adj[u].push_back(n + v);
            adj[n + v].push_back(u);
            capacity[u][n + v] = 1;
        }
        
        bool bfs(vector<int>& parent) {
            fill(parent.begin(), parent.end(), -1);
            parent[source] = -2;
            queue<pair<int, int>> q;
            q.push({source, INT_MAX});
            
            while (!q.empty()) {
                int cur = q.front().first;
                int flow = q.front().second;
                q.pop();
                
                for (int next : adj[cur]) {
                    if (parent[next] == -1 && capacity[cur][next] > 0) {
                        parent[next] = cur;
                        int new_flow = min(flow, capacity[cur][next]);
                        if (next == sink) {
                            return true;
                        }
                        q.push({next, new_flow});
                    }
                }
            }
            
            return false;
        }
        
        int maxMatching() {
            int max_flow = 0;
            vector<int> parent(n + m + 2);
            
            while (bfs(parent)) {
                int path_flow = INT_MAX;
                for (int s = sink; s != source; s = parent[s]) {
                    path_flow = min(path_flow, capacity[parent[s]][s]);
                }
                
                for (int s = sink; s != source; s = parent[s]) {
                    capacity[parent[s]][s] -= path_flow;
                    capacity[s][parent[s]] += path_flow;
                }
                
                max_flow += path_flow;
            }
            
            return max_flow;
        }
        
        vector<pair<int, int>> getMatching() {
            vector<pair<int, int>> matching;
            
            for (int u = 0; u < n; u++) {
                for (int v = n; v < n + m; v++) {
                    if (capacity[v][u] == 1) { // Reverse edge has flow
                        matching.push_back({u, v - n});
                    }
                }
            }
            
            return matching;
        }
    };
    
    // Weighted Bipartite Matching (Hungarian Algorithm)
    class WeightedMatching {
    public:
        vector<vector<long long>> cost;
        vector<long long> u, v;
        vector<int> p, way;
        int n;
        
        WeightedMatching(vector<vector<long long>>& cost_matrix) : cost(cost_matrix) {
            n = cost.size();
            u.resize(n + 1);
            v.resize(n + 1);
            p.resize(n + 1);
            way.resize(n + 1);
        }
        
        long long solve() {
            for (int i = 1; i <= n; ++i) {
                p[0] = i;
                int j0 = 0;
                vector<long long> minv(n + 1, LLONG_MAX);
                vector<bool> used(n + 1, false);
                
                do {
                    used[j0] = true;
                    int i0 = p[j0];
                    long long delta = LLONG_MAX;
                    int j1;
                    
                    for (int j = 1; j <= n; ++j) {
                        if (!used[j]) {
                            long long cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
                            if (cur < minv[j]) {
                                minv[j] = cur;
                                way[j] = j0;
                            }
                            if (minv[j] < delta) {
                                delta = minv[j];
                                j1 = j;
                            }
                        }
                    }
                    
                    for (int j = 0; j <= n; ++j) {
                        if (used[j]) {
                            u[p[j]] += delta;
                            v[j] -= delta;
                        } else {
                            minv[j] -= delta;
                        }
                    }
                    
                    j0 = j1;
                } while (p[j0] != 0);
                
                do {
                    int j1 = way[j0];
                    p[j0] = p[j1];
                    j0 = j1;
                } while (j0);
            }
            
            return -v[0];
        }
        
        vector<pair<int, int>> getMatching() {
            vector<pair<int, int>> matching;
            for (int j = 1; j <= n; ++j) {
                if (p[j] != 0) {
                    matching.push_back({p[j] - 1, j - 1});
                }
            }
            return matching;
        }
    };
    
    // General Graph Matching (Blossom Algorithm - simplified)
    class BlossomMatching {
    public:
        vector<vector<int>> graph;
        vector<int> match, base, parent, q;
        vector<bool> used, blossom;
        int n, qh, qt;
        
        BlossomMatching(int n) : n(n) {
            graph.resize(n);
            match.resize(n, -1);
            base.resize(n);
            parent.resize(n);
            q.resize(n);
            used.resize(n);
            blossom.resize(n);
        }
        
        void addEdge(int u, int v) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        
        int lca(int a, int b) {
            fill(used.begin(), used.end(), false);
            while (true) {
                a = base[a];
                used[a] = true;
                if (match[a] == -1) break;
                a = parent[match[a]];
            }
            while (true) {
                b = base[b];
                if (used[b]) return b;
                b = parent[match[b]];
            }
        }
        
        void markPath(int v, int b, int children) {
            while (base[v] != b) {
                blossom[base[v]] = blossom[base[match[v]]] = true;
                parent[v] = children;
                children = match[v];
                v = parent[match[v]];
            }
        }
        
        int findPath(int root) {
            fill(used.begin(), used.end(), false);
            fill(parent.begin(), parent.end(), -1);
            for (int i = 0; i < n; ++i) {
                base[i] = i;
            }
            
            used[root] = true;
            qh = qt = 0;
            q[qt++] = root;
            
            while (qh < qt) {
                int v = q[qh++];
                for (int to : graph[v]) {
                    if (base[v] == base[to] || match[v] == to) continue;
                    
                    if (to == root || (match[to] != -1 && parent[match[to]] != -1)) {
                        int curbase = lca(v, to);
                        fill(blossom.begin(), blossom.end(), false);
                        markPath(v, curbase, to);
                        markPath(to, curbase, v);
                        
                        for (int i = 0; i < n; ++i) {
                            if (blossom[base[i]]) {
                                base[i] = curbase;
                                if (!used[i]) {
                                    used[i] = true;
                                    q[qt++] = i;
                                }
                            }
                        }
                    } else if (parent[to] == -1) {
                        parent[to] = v;
                        if (match[to] == -1) {
                            return to;
                        }
                        to = match[to];
                        used[to] = true;
                        q[qt++] = to;
                    }
                }
            }
            
            return -1;
        }
        
        int maxMatching() {
            int result = 0;
            
            for (int v = 0; v < n; ++v) {
                if (match[v] == -1) {
                    int to = findPath(v);
                    if (to != -1) {
                        result++;
                        while (to != -1) {
                            int pv = parent[to];
                            int ppv = match[pv];
                            match[to] = pv;
                            match[pv] = to;
                            to = ppv;
                        }
                    }
                }
            }
            
            return result;
        }
        
        vector<pair<int, int>> getMatching() {
            vector<pair<int, int>> matching;
            for (int i = 0; i < n; i++) {
                if (match[i] != -1 && i < match[i]) {
                    matching.push_back({i, match[i]});
                }
            }
            return matching;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace BipartiteMatching;
    
    // Kuhn's algorithm example
    KuhnMatching kuhn(3, 3);
    kuhn.addEdge(0, 0);
    kuhn.addEdge(0, 1);
    kuhn.addEdge(1, 1);
    kuhn.addEdge(1, 2);
    kuhn.addEdge(2, 0);
    
    cout << "Maximum matching (Kuhn): " << kuhn.maxMatching() << endl;
    
    auto matching = kuhn.getMatching();
    cout << "Matching edges: ";
    for (auto [u, v] : matching) {
        cout << "(" << u << ", " << v << ") ";
    }
    cout << endl;
    
    // Hopcroft-Karp example
    HopcroftKarp hk(3, 3);
    hk.addEdge(0, 0);
    hk.addEdge(0, 1);
    hk.addEdge(1, 1);
    hk.addEdge(1, 2);
    hk.addEdge(2, 0);
    
    cout << "Maximum matching (Hopcroft-Karp): " << hk.maxMatching() << endl;
    
    // Weighted matching example
    vector<vector<long long>> cost_matrix = {
        {4, 1, 3},
        {2, 0, 5},
        {3, 2, 2}
    };
    
    WeightedMatching wm(cost_matrix);
    long long min_cost = wm.solve();
    cout << "Minimum cost matching: " << min_cost << endl;
    
    return 0;
}
*/