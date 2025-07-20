#include <bits/stdc++.h>
using namespace std;

// Maximum Flow Algorithms
namespace MaxFlow {
    
    // Dinic's Algorithm
    class Dinic {
    public:
        struct Edge {
            int to, cap, flow;
        };
        
        vector<Edge> edges;
        vector<vector<int>> graph;
        vector<int> level, iter;
        int n;
        
        Dinic(int n) : n(n) {
            graph.resize(n);
            level.resize(n);
            iter.resize(n);
        }
        
        void addEdge(int from, int to, int cap) {
            graph[from].push_back(edges.size());
            edges.push_back({to, cap, 0});
            graph[to].push_back(edges.size());
            edges.push_back({from, 0, 0});
        }
        
        bool bfs(int s, int t) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            queue<int> q;
            q.push(s);
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (int id : graph[v]) {
                    if (level[edges[id].to] < 0 && edges[id].flow < edges[id].cap) {
                        level[edges[id].to] = level[v] + 1;
                        q.push(edges[id].to);
                    }
                }
            }
            
            return level[t] >= 0;
        }
        
        int dfs(int v, int t, int pushed) {
            if (v == t || pushed == 0) {
                return pushed;
            }
            
            for (int& cid = iter[v]; cid < graph[v].size(); cid++) {
                int id = graph[v][cid];
                int to = edges[id].to;
                
                if (level[v] + 1 != level[to] || edges[id].cap <= edges[id].flow) {
                    continue;
                }
                
                int tr = dfs(to, t, min(pushed, edges[id].cap - edges[id].flow));
                if (tr > 0) {
                    edges[id].flow += tr;
                    edges[id ^ 1].flow -= tr;
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
                
                for (int id : graph[v]) {
                    if (!cut[edges[id].to] && edges[id].flow < edges[id].cap) {
                        cut[edges[id].to] = true;
                        q.push(edges[id].to);
                    }
                }
            }
            
            return cut;
        }
    };
    
    // Ford-Fulkerson with DFS
    class FordFulkerson {
    public:
        vector<vector<int>> capacity;
        vector<vector<int>> adj;
        int n;
        
        FordFulkerson(int n) : n(n) {
            capacity.assign(n, vector<int>(n, 0));
            adj.resize(n);
        }
        
        void addEdge(int from, int to, int cap) {
            adj[from].push_back(to);
            adj[to].push_back(from);
            capacity[from][to] += cap;
        }
        
        int dfs(int s, int t, int f, vector<bool>& visited) {
            if (s == t) return f;
            
            visited[s] = true;
            for (int to : adj[s]) {
                if (!visited[to] && capacity[s][to] > 0) {
                    int d = dfs(to, t, min(f, capacity[s][to]), visited);
                    if (d > 0) {
                        capacity[s][to] -= d;
                        capacity[to][s] += d;
                        return d;
                    }
                }
            }
            
            return 0;
        }
        
        long long maxFlow(int s, int t) {
            long long flow = 0;
            vector<bool> visited(n);
            
            while (true) {
                fill(visited.begin(), visited.end(), false);
                int f = dfs(s, t, INT_MAX, visited);
                if (f == 0) break;
                flow += f;
            }
            
            return flow;
        }
    };
    
    // Edmonds-Karp Algorithm (BFS-based Ford-Fulkerson)
    class EdmondsKarp {
    public:
        vector<vector<int>> capacity;
        vector<vector<int>> adj;
        int n;
        
        EdmondsKarp(int n) : n(n) {
            capacity.assign(n, vector<int>(n, 0));
            adj.resize(n);
        }
        
        void addEdge(int from, int to, int cap) {
            adj[from].push_back(to);
            adj[to].push_back(from);
            capacity[from][to] += cap;
        }
        
        int bfs(int s, int t, vector<int>& parent) {
            fill(parent.begin(), parent.end(), -1);
            parent[s] = -2;
            queue<pair<int, int>> q;
            q.push({s, INT_MAX});
            
            while (!q.empty()) {
                int cur = q.front().first;
                int flow = q.front().second;
                q.pop();
                
                for (int next : adj[cur]) {
                    if (parent[next] == -1 && capacity[cur][next] > 0) {
                        parent[next] = cur;
                        int new_flow = min(flow, capacity[cur][next]);
                        if (next == t) {
                            return new_flow;
                        }
                        q.push({next, new_flow});
                    }
                }
            }
            
            return 0;
        }
        
        long long maxFlow(int s, int t) {
            long long flow = 0;
            vector<int> parent(n);
            int new_flow;
            
            while (new_flow = bfs(s, t, parent)) {
                flow += new_flow;
                int cur = t;
                while (cur != s) {
                    int prev = parent[cur];
                    capacity[prev][cur] -= new_flow;
                    capacity[cur][prev] += new_flow;
                    cur = prev;
                }
            }
            
            return flow;
        }
    };
    
    // Push-Relabel Algorithm
    class PushRelabel {
    public:
        struct Edge {
            int to, cap, flow;
        };
        
        vector<Edge> edges;
        vector<vector<int>> graph;
        vector<int> height, excess, count;
        queue<int> excess_vertices;
        int n;
        
        PushRelabel(int n) : n(n) {
            graph.resize(n);
            height.resize(n);
            excess.resize(n);
            count.resize(2 * n);
        }
        
        void addEdge(int from, int to, int cap) {
            graph[from].push_back(edges.size());
            edges.push_back({to, cap, 0});
            graph[to].push_back(edges.size());
            edges.push_back({from, 0, 0});
        }
        
        void push(int v, int id) {
            int d = min(excess[v], edges[id].cap - edges[id].flow);
            edges[id].flow += d;
            edges[id ^ 1].flow -= d;
            excess[v] -= d;
            excess[edges[id].to] += d;
            
            if (d > 0 && excess[edges[id].to] == d) {
                excess_vertices.push(edges[id].to);
            }
        }
        
        void relabel(int v) {
            count[height[v]]--;
            height[v] = 2 * n;
            
            for (int id : graph[v]) {
                if (edges[id].cap > edges[id].flow) {
                    height[v] = min(height[v], height[edges[id].to] + 1);
                }
            }
            
            count[height[v]]++;
        }
        
        void discharge(int v) {
            for (int id : graph[v]) {
                if (excess[v] > 0 && edges[id].cap > edges[id].flow && height[v] == height[edges[id].to] + 1) {
                    push(v, id);
                }
            }
            
            if (excess[v] > 0) {
                if (count[height[v]] == 1) {
                    int old_height = height[v];
                    for (int u = 0; u < n; u++) {
                        if (height[u] > old_height) {
                            count[height[u]]--;
                            height[u] = n + 1;
                            count[height[u]]++;
                        }
                    }
                } else {
                    relabel(v);
                    excess_vertices.push(v);
                }
            }
        }
        
        long long maxFlow(int s, int t) {
            fill(height.begin(), height.end(), 0);
            fill(excess.begin(), excess.end(), 0);
            fill(count.begin(), count.end(), 0);
            
            height[s] = n;
            count[0] = n - 1;
            count[n] = 1;
            
            for (int id : graph[s]) {
                excess[s] += edges[id].cap;
                push(s, id);
            }
            
            while (!excess_vertices.empty()) {
                int v = excess_vertices.front();
                excess_vertices.pop();
                
                if (v != s && v != t && excess[v] > 0) {
                    discharge(v);
                }
            }
            
            return excess[t];
        }
    };
    
    // ISAP (Improved Shortest Augmenting Path)
    class ISAP {
    public:
        struct Edge {
            int to, cap, flow;
        };
        
        vector<Edge> edges;
        vector<vector<int>> graph;
        vector<int> level, iter, gap;
        int n;
        
        ISAP(int n) : n(n) {
            graph.resize(n);
            level.resize(n);
            iter.resize(n);
            gap.resize(n);
        }
        
        void addEdge(int from, int to, int cap) {
            graph[from].push_back(edges.size());
            edges.push_back({to, cap, 0});
            graph[to].push_back(edges.size());
            edges.push_back({from, 0, 0});
        }
        
        void bfs(int t) {
            fill(level.begin(), level.end(), -1);
            fill(gap.begin(), gap.end(), 0);
            
            level[t] = 0;
            gap[0] = 1;
            queue<int> q;
            q.push(t);
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (int id : graph[v]) {
                    int to = edges[id ^ 1].to;
                    if (level[to] == -1) {
                        level[to] = level[v] + 1;
                        gap[level[to]]++;
                        q.push(to);
                    }
                }
            }
        }
        
        int dfs(int v, int t, int f) {
            if (v == t) return f;
            
            int res = 0;
            for (int& i = iter[v]; i < graph[v].size(); i++) {
                int id = graph[v][i];
                int to = edges[id].to;
                
                if (edges[id].cap > edges[id].flow && level[v] == level[to] + 1) {
                    int d = dfs(to, t, min(f, edges[id].cap - edges[id].flow));
                    if (d > 0) {
                        edges[id].flow += d;
                        edges[id ^ 1].flow -= d;
                        res += d;
                        f -= d;
                        if (f == 0) break;
                    }
                }
            }
            
            if (res == 0) {
                gap[level[v]]--;
                if (gap[level[v]] == 0) {
                    for (int i = 0; i < n; i++) {
                        if (level[i] > level[v]) {
                            level[i] = n;
                        }
                    }
                }
                level[v] = n;
            }
            
            return res;
        }
        
        long long maxFlow(int s, int t) {
            bfs(t);
            if (level[s] == -1) return 0;
            
            long long flow = 0;
            while (level[s] < n) {
                fill(iter.begin(), iter.end(), 0);
                flow += dfs(s, t, INT_MAX);
            }
            
            return flow;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace MaxFlow;
    
    // Dinic's algorithm example
    Dinic dinic(6);
    dinic.addEdge(0, 1, 16);
    dinic.addEdge(0, 2, 13);
    dinic.addEdge(1, 2, 10);
    dinic.addEdge(1, 3, 12);
    dinic.addEdge(2, 1, 4);
    dinic.addEdge(2, 4, 14);
    dinic.addEdge(3, 2, 9);
    dinic.addEdge(3, 5, 20);
    dinic.addEdge(4, 3, 7);
    dinic.addEdge(4, 5, 4);
    
    cout << "Maximum flow (Dinic): " << dinic.maxFlow(0, 5) << endl;
    
    // Edmonds-Karp example
    EdmondsKarp ek(6);
    ek.addEdge(0, 1, 16);
    ek.addEdge(0, 2, 13);
    ek.addEdge(1, 2, 10);
    ek.addEdge(1, 3, 12);
    ek.addEdge(2, 1, 4);
    ek.addEdge(2, 4, 14);
    ek.addEdge(3, 2, 9);
    ek.addEdge(3, 5, 20);
    ek.addEdge(4, 3, 7);
    ek.addEdge(4, 5, 4);
    
    cout << "Maximum flow (Edmonds-Karp): " << ek.maxFlow(0, 5) << endl;
    
    return 0;
}
*/