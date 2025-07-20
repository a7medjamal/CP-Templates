#include <bits/stdc++.h>
using namespace std;

// Bitmask DP utilities and algorithms
class BitMaskDP {
public:
    static int popcount(int mask) {
        return __builtin_popcount(mask);
    }
    
    static int lowbit(int mask) {
        return mask & (-mask);
    }
    
    static void iterateSubsets(int mask, function<void(int)> func) {
        for (int submask = mask; ; submask = (submask - 1) & mask) {
            func(submask);
            if (submask == 0) break;
        }
    }
    
    static void iterateSupersets(int mask, int universe, function<void(int)> func) {
        for (int supermask = mask; supermask < (1 << universe); 
             supermask = (supermask + 1) | mask) {
            func(supermask);
        }
    }
    
    // Traveling Salesman Problem
    static long long tsp(vector<vector<long long>>& dist) {
        int n = dist.size();
        vector<vector<long long>> dp(1 << n, vector<long long>(n, LLONG_MAX));
        
        dp[1][0] = 0; // Start from city 0
        
        for (int mask = 1; mask < (1 << n); mask++) {
            for (int u = 0; u < n; u++) {
                if (!(mask & (1 << u)) || dp[mask][u] == LLONG_MAX) continue;
                
                for (int v = 0; v < n; v++) {
                    if (mask & (1 << v)) continue;
                    
                    int newMask = mask | (1 << v);
                    dp[newMask][v] = min(dp[newMask][v], dp[mask][u] + dist[u][v]);
                }
            }
        }
        
        long long result = LLONG_MAX;
        for (int i = 1; i < n; i++) {
            if (dp[(1 << n) - 1][i] != LLONG_MAX) {
                result = min(result, dp[(1 << n) - 1][i] + dist[i][0]);
            }
        }
        
        return result;
    }
    
    // Assignment Problem using bitmask DP
    static long long assignment(vector<vector<long long>>& cost) {
        int n = cost.size();
        vector<long long> dp(1 << n, LLONG_MAX);
        dp[0] = 0;
        
        for (int mask = 0; mask < (1 << n); mask++) {
            if (dp[mask] == LLONG_MAX) continue;
            
            int person = popcount(mask);
            if (person == n) continue;
            
            for (int task = 0; task < n; task++) {
                if (!(mask & (1 << task))) {
                    int newMask = mask | (1 << task);
                    dp[newMask] = min(dp[newMask], dp[mask] + cost[person][task]);
                }
            }
        }
        
        return dp[(1 << n) - 1];
    }
    
    // Maximum Weight Independent Set in a tree
    static long long maxWeightIndependentSet(vector<vector<int>>& adj, vector<long long>& weight) {
        int n = adj.size();
        vector<vector<long long>> dp(n, vector<long long>(2, 0));
        vector<bool> visited(n, false);
        
        function<void(int, int)> dfs = [&](int u, int parent) {
            visited[u] = true;
            dp[u][1] = weight[u]; // Include u
            dp[u][0] = 0; // Exclude u
            
            for (int v : adj[u]) {
                if (v != parent) {
                    dfs(v, u);
                    dp[u][1] += dp[v][0]; // If u is included, v must be excluded
                    dp[u][0] += max(dp[v][0], dp[v][1]); // If u is excluded, v can be either
                }
            }
        };
        
        dfs(0, -1);
        return max(dp[0][0], dp[0][1]);
    }
    
    // Count number of ways to tile 2xN board with 1x2 dominoes
    static long long tiling2xN(int n) {
        if (n == 0) return 1;
        if (n == 1) return 1;
        
        vector<long long> dp(n + 1);
        dp[0] = 1;
        dp[1] = 1;
        
        for (int i = 2; i <= n; i++) {
            dp[i] = dp[i-1] + dp[i-2];
        }
        
        return dp[n];
    }
    
    // Broken profile DP for complex tiling
    static long long complexTiling(int n, int m, vector<string>& grid) {
        map<int, long long> dp;
        dp[0] = 1;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                map<int, long long> newDp;
                
                for (auto& [mask, ways] : dp) {
                    if (grid[i][j] == '#') {
                        // Blocked cell
                        if (mask & (1 << j)) {
                            newDp[mask ^ (1 << j)] += ways;
                        } else {
                            newDp[mask] += ways;
                        }
                    } else {
                        // Free cell
                        if (mask & (1 << j)) {
                            // Already filled
                            newDp[mask ^ (1 << j)] += ways;
                        } else {
                            // Place vertical domino
                            if (i + 1 < n && grid[i+1][j] == '.') {
                                newDp[mask | (1 << j)] += ways;
                            }
                            // Place horizontal domino
                            if (j + 1 < m && grid[i][j+1] == '.' && !(mask & (1 << (j+1)))) {
                                newDp[mask] += ways;
                                j++; // Skip next cell
                            }
                        }
                    }
                }
                
                dp = newDp;
            }
        }
        
        return dp[0];
    }
};

// Example usage:
/*
int main() {
    // TSP example
    vector<vector<long long>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    cout << "TSP cost: " << BitMaskDP::tsp(dist) << endl;
    
    // Assignment problem
    vector<vector<long long>> cost = {
        {9, 2, 7, 8},
        {6, 4, 3, 7},
        {5, 8, 1, 8},
        {7, 6, 9, 4}
    };
    cout << "Assignment cost: " << BitMaskDP::assignment(cost) << endl;
    
    return 0;
}
*/