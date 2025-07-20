#include <bits/stdc++.h>
using namespace std;

// Advanced Graph Algorithms - Network Flows and Matching
namespace NetworkFlows {
    
    // Dinic's Algorithm for Maximum Flow
    class Dinic {
    public:
        struct Edge {
            int to, cap, flow, rev;
        };
        
        vector<vector<Edge>> graph;
        vector<int> level, iter;
        int n;
        
        Dinic(int n) : n(n) {
            graph.resize(n);
            level.resize(n);
            iter.resize(n);
        }
        
        void addEdge(int from, int to, int cap) {
            graph[from].push_back({to, cap, 0, (int)graph[to].size()});
            graph[to].push_back({from, 0, 0, (int)graph[from].size() - 1});
        }
        
        bool bfs(int s, int t) {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            level[s] = 0;
            q.push(s);
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (auto& e : graph[v]) {
                    if (level[e.to] < 0 && e.flow < e.cap) {
                        level[e.to] = level[v] + 1;
                        q.push(e.to);
                    }
                }
            }
            
            return level[t] >= 0;
        }
        
        int dfs(int v, int t, int pushed) {
            if (v == t || pushed == 0) return pushed;
            
            for (int& cid = iter[v]; cid < graph[v].size(); cid++) {
                Edge& e = graph[v][cid];
                
                if (level[v] + 1 != level[e.to] || e.cap <= e.flow)
                    continue;
                
                int tr = dfs(e.to, t, min(pushed, e.cap - e.flow));
                if (tr > 0) {
                    e.flow += tr;
                    graph[e.to][e.rev].flow -= tr;
                    return tr;
                }
            }
            
            return 0;
        }
        
        long long maxFlow(int s, int t) {
            long long flow = 0;
            
            while (bfs(s, t)) {
                fill(iter.begin(), iter.end(), 0);
                while (int pushed = dfs(s, t, INT_MAX)) {
                    flow += pushed;
                }
            }
            
            return flow;
        }
        
        vector<bool> minCut(int s) {
            vector<bool> cut(n, false);
            queue<int> q;
            q.push(s);
            cut[s] = true;
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (auto& e : graph[v]) {
                    if (!cut[e.to] && e.flow < e.cap) {
                        cut[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
            
            return cut;
        }
    };
    
    // Min Cost Max Flow using SPFA
    class MinCostMaxFlow {
    public:
        struct Edge {
            int to, cap, cost, flow, rev;
        };
        
        vector<vector<Edge>> graph;
        vector<long long> dist;
        vector<int> parent, parentEdge;
        vector<bool> inQueue;
        int n;
        
        MinCostMaxFlow(int n) : n(n) {
            graph.resize(n);
            dist.resize(n);
            parent.resize(n);
            parentEdge.resize(n);
            inQueue.resize(n);
        }
        
        void addEdge(int from, int to, int cap, int cost) {
            graph[from].push_back({to, cap, cost, 0, (int)graph[to].size()});
            graph[to].push_back({from, 0, -cost, 0, (int)graph[from].size() - 1});
        }
        
        bool spfa(int s, int t) {
            fill(dist.begin(), dist.end(), LLONG_MAX);
            fill(inQueue.begin(), inQueue.end(), false);
            
            queue<int> q;
            dist[s] = 0;
            q.push(s);
            inQueue[s] = true;
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                inQueue[v] = false;
                
                for (int i = 0; i < graph[v].size(); i++) {
                    Edge& e = graph[v][i];
                    
                    if (e.cap > e.flow && dist[v] + e.cost < dist[e.to]) {
                        dist[e.to] = dist[v] + e.cost;
                        parent[e.to] = v;
                        parentEdge[e.to] = i;
                        
                        if (!inQueue[e.to]) {
                            q.push(e.to);
                            inQueue[e.to] = true;
                        }
                    }
                }
            }
            
            return dist[t] != LLONG_MAX;
        }
        
        pair<long long, long long> minCostMaxFlow(int s, int t) {
            long long flow = 0, cost = 0;
            
            while (spfa(s, t)) {
                int pushFlow = INT_MAX;
                
                for (int v = t; v != s; v = parent[v]) {
                    Edge& e = graph[parent[v]][parentEdge[v]];
                    pushFlow = min(pushFlow, e.cap - e.flow);
                }
                
                for (int v = t; v != s; v = parent[v]) {
                    Edge& e = graph[parent[v]][parentEdge[v]];
                    e.flow += pushFlow;
                    graph[v][e.rev].flow -= pushFlow;
                }
                
                flow += pushFlow;
                cost += pushFlow * dist[t];
            }
            
            return {flow, cost};
        }
    };
    
    // Hungarian Algorithm for Assignment Problem
    class Hungarian {
    public:
        vector<vector<long long>> cost;
        vector<long long> u, v;
        vector<int> p, way;
        int n;
        
        Hungarian(vector<vector<long long>>& costMatrix) : cost(costMatrix) {
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
        
        vector<int> getAssignment() {
            vector<int> assignment(n);
            for (int j = 1; j <= n; ++j) {
                assignment[p[j] - 1] = j - 1;
            }
            return assignment;
        }
    };
    
    // Note: Bipartite matching algorithms have been moved to bipartite_matching.cpp
    
    // General Maximum Matching (Blossom Algorithm - simplified)
    class GeneralMatching {
    public:
        vector<vector<int>> graph;
        vector<int> match, base, p, used, a, b;
        int n, timestamp;
        
        GeneralMatching(int n) : n(n) {
            graph.resize(n);
            match.resize(n, -1);
            base.resize(n);
            p.resize(n);
            used.resize(n);
            a.resize(n);
            b.resize(n);
            timestamp = 0;
        }
        
        void addEdge(int u, int v) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        
        int lca(int a, int b) {
            ++timestamp;
            while (true) {
                if (a != -1) {
                    if (used[a] == timestamp) return a;
                    used[a] = timestamp;
                    a = (match[a] == -1) ? -1 : p[match[a]];
                }
                swap(a, b);
            }
        }
        
        void markPath(vector<int>& v, int x, int b, int children) {
            while (base[x] != b) {
                v.push_back(x);
                v.push_back(match[x]);
                p[match[x]] = children;
                x = p[match[x]];
            }
        }
        
        int findPath(int root) {
            fill(used.begin(), used.end(), -1);
            iota(base.begin(), base.end(), 0);
            fill(p.begin(), p.end(), -1);
            
            queue<int> q;
            q.push(root);
            used[root] = root;
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (int to : graph[v]) {
                    if (base[v] == base[to] || match[v] == to) continue;
                    
                    if (to == root || (match[to] != -1 && p[match[to]] != -1)) {
                        int curbase = lca(v, to);
                        vector<int> blossom;
                        markPath(blossom, v, curbase, to);
                        markPath(blossom, to, curbase, v);
                        
                        for (int i : blossom) {
                            base[i] = curbase;
                            if (used[i] == -1) {
                                used[i] = root;
                                q.push(i);
                            }
                        }
                    } else if (p[to] == -1) {
                        p[to] = v;
                        if (match[to] == -1) return to;
                        
                        to = match[to];
                        used[to] = root;
                        q.push(to);
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
                        ++result;
                        while (to != -1) {
                            int pv = p[to], ppv = match[pv];
                            match[to] = pv;
                            match[pv] = to;
                            to = ppv;
                        }
                    }
                }
            }
            
            return result;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace NetworkFlows;
    
    // Dinic's algorithm example
    Dinic flow(6);
    flow.addEdge(0, 1, 10);
    flow.addEdge(0, 2, 10);
    flow.addEdge(1, 2, 2);
    flow.addEdge(1, 3, 4);
    flow.addEdge(1, 4, 8);
    flow.addEdge(2, 4, 9);
    flow.addEdge(3, 5, 10);
    flow.addEdge(4, 3, 6);
    flow.addEdge(4, 5, 10);
    
    cout << "Maximum flow: " << flow.maxFlow(0, 5) << endl;
    
    // Hungarian algorithm example
    vector<vector<long long>> cost = {
        {4, 1, 3},
        {2, 0, 5},
        {3, 2, 2}
    };
    
    Hungarian hungarian(cost);
    cout << "Minimum assignment cost: " << hungarian.solve() << endl;
    
    return 0;
}
*/