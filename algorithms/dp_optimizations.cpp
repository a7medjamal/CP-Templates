#include <bits/stdc++.h>
using namespace std;

// Divide and Conquer DP Optimization
class DivideConquerDP {
public:
    vector<vector<long long>> dp;
    vector<long long> cost;
    int n, k;
    
    DivideConquerDP(vector<long long>& arr, int groups) : cost(arr), n(arr.size()), k(groups) {
        dp.assign(k + 1, vector<long long>(n, LLONG_MAX));
    }
    
    long long getCost(int l, int r) {
        // Implement your cost function here
        // Example: sum of elements from l to r
        long long sum = 0;
        for (int i = l; i <= r; i++) {
            sum += cost[i];
        }
        return sum;
    }
    
    void compute(int g, int l, int r, int optl, int optr) {
        if (l > r) return;
        
        int mid = (l + r) / 2;
        pair<long long, int> best = {LLONG_MAX, -1};
        
        for (int k = optl; k <= min(mid, optr); k++) {
            if (dp[g-1][k-1] != LLONG_MAX) {
                long long val = dp[g-1][k-1] + getCost(k, mid);
                best = min(best, {val, k});
            }
        }
        
        dp[g][mid] = best.first;
        int opt = best.second;
        
        compute(g, l, mid - 1, optl, opt);
        compute(g, mid + 1, r, opt, optr);
    }
    
    long long solve() {
        // Base case
        for (int i = 0; i < n; i++) {
            dp[1][i] = getCost(0, i);
        }
        
        for (int g = 2; g <= k; g++) {
            compute(g, 0, n - 1, 0, n - 1);
        }
        
        return dp[k][n-1];
    }
};

// Knuth-Yao Optimization for Matrix Chain-like problems
class KnuthYaoDP {
public:
    vector<vector<long long>> dp, opt;
    vector<long long> prefix;
    int n;
    
    KnuthYaoDP(vector<long long>& arr) : n(arr.size()) {
        dp.assign(n, vector<long long>(n, LLONG_MAX));
        opt.assign(n, vector<long long>(n, 0));
        prefix.resize(n + 1, 0);
        
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + arr[i];
        }
    }
    
    long long cost(int l, int r) {
        return prefix[r + 1] - prefix[l];
    }
    
    long long solve() {
        // Base case
        for (int i = 0; i < n; i++) {
            dp[i][i] = 0;
            opt[i][i] = i;
        }
        
        for (int len = 2; len <= n; len++) {
            for (int l = 0; l <= n - len; l++) {
                int r = l + len - 1;
                
                for (int k = opt[l][r-1]; k <= opt[l+1][r]; k++) {
                    long long val = dp[l][k] + dp[k+1][r] + cost(l, r);
                    if (val < dp[l][r]) {
                        dp[l][r] = val;
                        opt[l][r] = k;
                    }
                }
            }
        }
        
        return dp[0][n-1];
    }
};

// Matrix Chain Multiplication with optimizations
class MatrixChainDP {
public:
    vector<int> dims;
    vector<vector<long long>> dp;
    vector<vector<int>> split;
    
    MatrixChainDP(vector<int>& dimensions) : dims(dimensions) {
        int n = dimensions.size() - 1;
        dp.assign(n, vector<long long>(n, 0));
        split.assign(n, vector<int>(n, 0));
    }
    
    long long solve() {
        int n = dims.size() - 1;
        
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                dp[i][j] = LLONG_MAX;
                
                for (int k = i; k < j; k++) {
                    long long cost = dp[i][k] + dp[k+1][j] + 
                                   (long long)dims[i] * dims[k+1] * dims[j+1];
                    
                    if (cost < dp[i][j]) {
                        dp[i][j] = cost;
                        split[i][j] = k;
                    }
                }
            }
        }
        
        return dp[0][n-1];
    }
    
    void printOptimalParens(int i, int j) {
        if (i == j) {
            cout << "M" << i;
        } else {
            cout << "(";
            printOptimalParens(i, split[i][j]);
            printOptimalParens(split[i][j] + 1, j);
            cout << ")";
        }
    }
};

// Monotonic Queue Optimization for DP
class MonotonicQueueDP {
public:
    static vector<long long> slidingWindowMaximum(vector<long long>& arr, int k) {
        int n = arr.size();
        vector<long long> result;
        deque<int> dq;
        
        for (int i = 0; i < n; i++) {
            // Remove elements outside window
            while (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }
            
            // Remove smaller elements
            while (!dq.empty() && arr[dq.back()] <= arr[i]) {
                dq.pop_back();
            }
            
            dq.push_back(i);
            
            if (i >= k - 1) {
                result.push_back(arr[dq.front()]);
            }
        }
        
        return result;
    }
    
    // DP with monotonic queue optimization
    static long long dpWithMonotonicQueue(vector<long long>& arr, int k) {
        int n = arr.size();
        vector<long long> dp(n + 1, LLONG_MIN);
        dp[0] = 0;
        
        deque<pair<long long, int>> dq;
        dq.push_back({0, 0});
        
        for (int i = 1; i <= n; i++) {
            // Remove elements outside window
            while (!dq.empty() && dq.front().second < i - k) {
                dq.pop_front();
            }
            
            if (!dq.empty()) {
                dp[i] = dq.front().first + arr[i-1];
            }
            
            // Maintain monotonic property
            while (!dq.empty() && dq.back().first <= dp[i]) {
                dq.pop_back();
            }
            
            dq.push_back({dp[i], i});
        }
        
        return dp[n];
    }
};

// Example usage:
/*
int main() {
    // Divide and Conquer DP example
    vector<long long> arr = {1, 2, 3, 4, 5};
    DivideConquerDP dcDP(arr, 3);
    cout << "Divide and Conquer DP result: " << dcDP.solve() << endl;
    
    // Knuth-Yao DP example
    KnuthYaoDP kyDP(arr);
    cout << "Knuth-Yao DP result: " << kyDP.solve() << endl;
    
    // Matrix Chain example
    vector<int> dims = {1, 2, 3, 4, 5};
    MatrixChainDP mcDP(dims);
    cout << "Matrix Chain result: " << mcDP.solve() << endl;
    
    return 0;
}
*/