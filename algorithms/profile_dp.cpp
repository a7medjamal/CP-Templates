#include <bits/stdc++.h>
using namespace std;

// Profile DP for grid-based problems
class ProfileDP {
public:
    int n, m;
    vector<map<int, long long>> dp;
    
    ProfileDP(int rows, int cols) : n(rows), m(cols) {
        dp.resize(n * m + 1);
    }
    
    // Count ways to tile n x m grid with 1x2 dominoes
    long long countTilings() {
        dp[0][0] = 1;
        
        for (int pos = 0; pos < n * m; pos++) {
            for (auto& [mask, ways] : dp[pos]) {
                int row = pos / m;
                int col = pos % m;
                
                if (mask & (1 << col)) {
                    // Cell is already filled by previous domino
                    dp[pos + 1][mask ^ (1 << col)] += ways;
                } else {
                    // Place vertical domino
                    if (row + 1 < n) {
                        dp[pos + 1][mask | (1 << col)] += ways;
                    }
                    
                    // Place horizontal domino
                    if (col + 1 < m && !(mask & (1 << (col + 1)))) {
                        dp[pos + 1][mask] += ways;
                    }
                }
            }
        }
        
        return dp[n * m][0];
    }
    
    // Count ways to tile with obstacles
    long long countTilingsWithObstacles(vector<string>& grid) {
        dp.assign(n * m + 1, map<int, long long>());
        dp[0][0] = 1;
        
        for (int pos = 0; pos < n * m; pos++) {
            for (auto& [mask, ways] : dp[pos]) {
                int row = pos / m;
                int col = pos % m;
                
                if (grid[row][col] == '#') {
                    // Blocked cell
                    if (mask & (1 << col)) {
                        dp[pos + 1][mask ^ (1 << col)] += ways;
                    } else {
                        dp[pos + 1][mask] += ways;
                    }
                } else {
                    // Free cell
                    if (mask & (1 << col)) {
                        // Already filled
                        dp[pos + 1][mask ^ (1 << col)] += ways;
                    } else {
                        // Place vertical domino
                        if (row + 1 < n && grid[row + 1][col] == '.') {
                            dp[pos + 1][mask | (1 << col)] += ways;
                        }
                        
                        // Place horizontal domino
                        if (col + 1 < m && grid[row][col + 1] == '.' && !(mask & (1 << (col + 1)))) {
                            dp[pos + 1][mask] += ways;
                        }
                    }
                }
            }
        }
        
        return dp[n * m][0];
    }
    
    // Count ways to place non-attacking rooks
    long long countNonAttackingRooks(int rooks) {
        if (rooks > min(n, m)) return 0;
        
        dp.assign(n + 1, map<int, long long>());
        dp[0][0] = 1;
        
        for (int row = 0; row < n; row++) {
            for (auto& [mask, ways] : dp[row]) {
                int placed = __builtin_popcount(mask);
                
                // Don't place any rook in this row
                dp[row + 1][mask] += ways;
                
                // Place a rook in this row
                if (placed < rooks) {
                    for (int col = 0; col < m; col++) {
                        if (!(mask & (1 << col))) {
                            dp[row + 1][mask | (1 << col)] += ways;
                        }
                    }
                }
            }
        }
        
        long long result = 0;
        for (auto& [mask, ways] : dp[n]) {
            if (__builtin_popcount(mask) == rooks) {
                result += ways;
            }
        }
        
        return result;
    }
    
    // Broken profile DP for complex shapes
    long long countComplexTilings(vector<string>& grid, vector<pair<int, int>>& shapes) {
        dp.assign(n * m + 1, map<int, long long>());
        dp[0][0] = 1;
        
        for (int pos = 0; pos < n * m; pos++) {
            for (auto& [mask, ways] : dp[pos]) {
                int row = pos / m;
                int col = pos % m;
                
                if (grid[row][col] == '#' || (mask & (1 << col))) {
                    // Blocked or already filled
                    dp[pos + 1][mask & ~(1 << col)] += ways;
                } else {
                    // Try each shape
                    for (auto& [dr, dc] : shapes) {
                        int newRow = row + dr;
                        int newCol = col + dc;
                        
                        if (newRow < n && newCol < m && grid[newRow][newCol] == '.') {
                            if (dr == 0) { // Horizontal
                                if (!(mask & (1 << newCol))) {
                                    dp[pos + 1][mask] += ways;
                                }
                            } else { // Vertical
                                dp[pos + 1][mask | (1 << col)] += ways;
                            }
                        }
                    }
                }
            }
        }
        
        return dp[n * m][0];
    }
};

// Optimized Profile DP for large grids
class OptimizedProfileDP {
public:
    static long long countTilings(int n, int m) {
        if (n > m) swap(n, m);
        
        vector<long long> dp(1 << n, 0);
        dp[0] = 1;
        
        for (int col = 0; col < m; col++) {
            vector<long long> newDp(1 << n, 0);
            
            for (int mask = 0; mask < (1 << n); mask++) {
                if (dp[mask] == 0) continue;
                
                fill(mask, 0, 0, newDp, dp[mask]);
            }
            
            dp = newDp;
        }
        
        return dp[0];
    }
    
private:
    static void fill(int mask, int pos, int nextMask, vector<long long>& newDp, long long ways) {
        int n = __builtin_ctz(newDp.size()); // log2 of size
        
        if (pos == n) {
            newDp[nextMask] += ways;
            return;
        }
        
        if (mask & (1 << pos)) {
            // Already filled
            fill(mask, pos + 1, nextMask, newDp, ways);
        } else {
            // Place vertical domino
            fill(mask | (1 << pos), pos + 1, nextMask | (1 << pos), newDp, ways);
            
            // Place horizontal domino
            if (pos + 1 < n && !(mask & (1 << (pos + 1)))) {
                fill(mask | (1 << pos) | (1 << (pos + 1)), pos + 2, nextMask, newDp, ways);
            }
        }
    }
};

// Example usage:
/*
int main() {
    ProfileDP pdp(3, 4);
    cout << "Tilings for 3x4 grid: " << pdp.countTilings() << endl;
    
    vector<string> grid = {
        "...",
        ".#.",
        "..."
    };
    ProfileDP pdp2(3, 3);
    cout << "Tilings with obstacles: " << pdp2.countTilingsWithObstacles(grid) << endl;
    
    cout << "Optimized tilings for 4x5: " << OptimizedProfileDP::countTilings(4, 5) << endl;
    
    return 0;
}
*/