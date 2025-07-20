#include <bits/stdc++.h>
using namespace std;

// Minimum Cost Flow and Assignment Algorithms
namespace MinCostFlow {
    
    // Min Cost Max Flow using SPFA
    class MinCostMaxFlow {
    public:
        struct Edge {
            int to, cap, cost, flow;
        };
        
        vector<Edge> edges;
        vector<vector<int>> graph;
        vector<int> dist, parent, parent_edge;
        vector<bool> inqueue;
        int n;
        
        MinCostMaxFlow(int n) : n(n) {
            graph.resize(n);
            dist.resize(n);
            parent.resize(n);
            parent_edge.resize(n);
            inqueue.resize(n);
        }
        
        void addEdge(int from, int to, int cap, int cost) {
            graph[from].push_back(edges.size());
            edges.push_back({to, cap, cost, 0});
            graph[to].push_back(edges.size());
            edges.push_back({from, 0, -cost, 0});
        }
        
        bool spfa(int s, int t) {
            fill(dist.begin(), dist.end(), INT_MAX);
            fill(inqueue.begin(), inqueue.end(), false);
            
            dist[s] = 0;
            queue<int> q;
            q.push(s);
            inqueue[s] = true;
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                inqueue[v] = false;
                
                for (int id : graph[v]) {
                    if (edges[id].cap > edges[id].flow && dist[v] + edges[id].cost < dist[edges[id].to]) {
                        dist[edges[id].to] = dist[v] + edges[id].cost;
                        parent[edges[id].to] = v;
                        parent_edge[edges[id].to] = id;
                        
                        if (!inqueue[edges[id].to]) {
                            q.push(edges[id].to);
                            inqueue[edges[id].to] = true;
                        }
                    }
                }
            }
            
            return dist[t] != INT_MAX;
        }
        
        pair<long long, long long> minCostMaxFlow(int s, int t) {
            long long flow = 0, cost = 0;
            
            while (spfa(s, t)) {
                int push = INT_MAX;
                for (int v = t; v != s; v = parent[v]) {
                    push = min(push, edges[parent_edge[v]].cap - edges[parent_edge[v]].flow);
                }
                
                for (int v = t; v != s; v = parent[v]) {
                    edges[parent_edge[v]].flow += push;
                    edges[parent_edge[v] ^ 1].flow -= push;
                }
                
                flow += push;
                cost += (long long)push * dist[t];
            }
            
            return {flow, cost};
        }
        
        long long minCostFlow(int s, int t, long long maxFlow) {
            long long cost = 0;
            long long flow = 0;
            
            while (flow < maxFlow && spfa(s, t)) {
                int push = min((long long)INT_MAX, maxFlow - flow);
                
                for (int v = t; v != s; v = parent[v]) {
                    push = min(push, (long long)(edges[parent_edge[v]].cap - edges[parent_edge[v]].flow));
                }
                
                for (int v = t; v != s; v = parent[v]) {
                    edges[parent_edge[v]].flow += push;
                    edges[parent_edge[v] ^ 1].flow -= push;
                }
                
                flow += push;
                cost += push * dist[t];
            }
            
            return cost;
        }
    };
    
    // Min Cost Max Flow using Dijkstra with potentials
    class MinCostMaxFlowDijkstra {
    public:
        struct Edge {
            int to, cap, cost, flow;
        };
        
        vector<Edge> edges;
        vector<vector<int>> graph;
        vector<long long> dist, potential;
        vector<int> parent, parent_edge;
        int n;
        
        MinCostMaxFlowDijkstra(int n) : n(n) {
            graph.resize(n);
            dist.resize(n);
            potential.resize(n);
            parent.resize(n);
            parent_edge.resize(n);
        }
        
        void addEdge(int from, int to, int cap, int cost) {
            graph[from].push_back(edges.size());
            edges.push_back({to, cap, cost, 0});
            graph[to].push_back(edges.size());
            edges.push_back({from, 0, -cost, 0});
        }
        
        bool dijkstra(int s, int t) {
            fill(dist.begin(), dist.end(), LLONG_MAX);
            fill(parent.begin(), parent.end(), -1);
            
            dist[s] = 0;
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
            pq.push({0, s});
            
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();
                
                if (d > dist[v]) continue;
                
                for (int id : graph[v]) {
                    int to = edges[id].to;
                    long long cost = edges[id].cost + potential[v] - potential[to];
                    
                    if (edges[id].cap > edges[id].flow && dist[v] + cost < dist[to]) {
                        dist[to] = dist[v] + cost;
                        parent[to] = v;
                        parent_edge[to] = id;
                        pq.push({dist[to], to});
                    }
                }
            }
            
            if (dist[t] == LLONG_MAX) return false;
            
            for (int i = 0; i < (int)n; i++) {
                if (dist[i] != LLONG_MAX) {
                    potential[i] += dist[i];
                }
            }
            
            return true;
        }
        
        pair<long long, long long> minCostMaxFlow(int s, int t) {
            fill(potential.begin(), potential.end(), 0);
            
            // Initial SPFA to set potentials
            vector<long long> d(n, LLONG_MAX);
            vector<bool> inqueue(n, false);
            d[s] = 0;
            queue<int> q;
            q.push(s);
            inqueue[s] = true;
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                inqueue[v] = false;
                
                for (int id : graph[v]) {
                    if (edges[id].cap > 0 && d[v] + edges[id].cost < d[edges[id].to]) {
                        d[edges[id].to] = d[v] + edges[id].cost;
                        if (!inqueue[edges[id].to]) {
                            q.push(edges[id].to);
                            inqueue[edges[id].to] = true;
                        }
                    }
                }
            }
            
            potential = d;
            
            long long flow = 0, cost = 0;
            
            while (dijkstra(s, t)) {
                int push = INT_MAX;
                for (int v = t; v != s; v = parent[v]) {
                    push = min(push, edges[parent_edge[v]].cap - edges[parent_edge[v]].flow);
                }
                
                for (int v = t; v != s; v = parent[v]) {
                    edges[parent_edge[v]].flow += push;
                    edges[parent_edge[v] ^ 1].flow -= push;
                }
                
                flow += push;
                cost += (long long)push * potential[t];
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
        
        Hungarian(vector<vector<long long>>& cost_matrix) : cost(cost_matrix) {
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
    
    // Cycle Canceling Algorithm
    class CycleCanceling {
    public:
        struct Edge {
            int to, cap, cost, flow;
        };
        
        vector<Edge> edges;
        vector<vector<int>> graph;
        vector<long long> dist;
        vector<int> parent, parent_edge;
        int n;
        
        CycleCanceling(int n) : n(n) {
            graph.resize(n);
            dist.resize(n);
            parent.resize(n);
            parent_edge.resize(n);
        }
        
        void addEdge(int from, int to, int cap, int cost) {
            graph[from].push_back(edges.size());
            edges.push_back({to, cap, cost, 0});
            graph[to].push_back(edges.size());
            edges.push_back({from, 0, -cost, 0});
        }
        
        bool bellmanFord() {
            fill(dist.begin(), dist.end(), 0);
            
            for (int i = 0; i < n; i++) {
                bool updated = false;
                for (int v = 0; v < n; v++) {
                    for (int id : graph[v]) {
                        if (edges[id].cap > edges[id].flow && dist[v] + edges[id].cost < dist[edges[id].to]) {
                            dist[edges[id].to] = dist[v] + edges[id].cost;
                            parent[edges[id].to] = v;
                            parent_edge[edges[id].to] = id;
                            updated = true;
                        }
                    }
                }
                if (!updated) break;
            }
            
            // Check for negative cycles
            for (int v = 0; v < n; v++) {
                for (int id : graph[v]) {
                    if (edges[id].cap > edges[id].flow && dist[v] + edges[id].cost < dist[edges[id].to]) {
                        return true; // Negative cycle found
                    }
                }
            }
            
            return false;
        }
        
        void cancelCycle(int start) {
            int v = start;
            int flow = INT_MAX;
            
            // Find minimum residual capacity in the cycle
            do {
                flow = min(flow, edges[parent_edge[v]].cap - edges[parent_edge[v]].flow);
                v = parent[v];
            } while (v != start);
            
            // Push flow through the cycle
            v = start;
            do {
                edges[parent_edge[v]].flow += flow;
                edges[parent_edge[v] ^ 1].flow -= flow;
                v = parent[v];
            } while (v != start);
        }
        
        long long minCostFlow(int s, int t, long long maxFlow) {
            // First, find maximum flow
            long long flow = 0;
            vector<long long> d(n);
            vector<int> p(n), pe(n);
            
            while (flow < maxFlow) {
                fill(d.begin(), d.end(), LLONG_MAX);
                d[s] = 0;
                queue<int> q;
                q.push(s);
                
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();
                    
                    for (int id : graph[v]) {
                        if (edges[id].cap > edges[id].flow && d[v] + edges[id].cost < d[edges[id].to]) {
                            d[edges[id].to] = d[v] + edges[id].cost;
                            p[edges[id].to] = v;
                            pe[edges[id].to] = id;
                            q.push(edges[id].to);
                        }
                    }
                }
                
                if (d[t] == LLONG_MAX) break;
                
                int push = min((long long)INT_MAX, maxFlow - flow);
                for (int v = t; v != s; v = p[v]) {
                    push = min(push, (long long)(edges[pe[v]].cap - edges[pe[v]].flow));
                }
                
                for (int v = t; v != s; v = p[v]) {
                    edges[pe[v]].flow += push;
                    edges[pe[v] ^ 1].flow -= push;
                }
                
                flow += push;
            }
            
            // Cancel negative cycles
            while (bellmanFord()) {
                // Find a vertex in a negative cycle
                int start = -1;
                for (int v = 0; v < n; v++) {
                    for (int id : graph[v]) {
                        if (edges[id].cap > edges[id].flow && dist[v] + edges[id].cost < dist[edges[id].to]) {
                            start = edges[id].to;
                            break;
                        }
                    }
                    if (start != -1) break;
                }
                
                if (start != -1) {
                    cancelCycle(start);
                }
            }
            
            long long cost = 0;
            for (const auto& edge : edges) {
                cost += (long long)edge.flow * edge.cost;
            }
            
            return cost / 2; // Each edge is counted twice
        }
    };
}

// Example usage:
/*
int main() {
    using namespace MinCostFlow;
    
    // Min Cost Max Flow example
    MinCostMaxFlow mcmf(4);
    mcmf.addEdge(0, 1, 2, 1);
    mcmf.addEdge(0, 2, 2, 2);
    mcmf.addEdge(1, 3, 2, 2);
    mcmf.addEdge(2, 3, 2, 1);
    
    auto [flow, cost] = mcmf.minCostMaxFlow(0, 3);
    cout << "Max flow: " << flow << ", Min cost: " << cost << endl;
    
    // Hungarian algorithm example
    vector<vector<long long>> cost_matrix = {
        {4, 1, 3},
        {2, 0, 5},
        {3, 2, 2}
    };
    
    Hungarian hungarian(cost_matrix);
    long long min_cost = hungarian.solve();
    auto assignment = hungarian.getAssignment();
    
    cout << "Assignment cost: " << min_cost << endl;
    cout << "Assignment: ";
    for (int i = 0; i < assignment.size(); i++) {
        cout << "(" << i << ", " << assignment[i] << ") ";
    }
    cout << endl;
    
    return 0;
}
*/