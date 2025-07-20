#include <bits/stdc++.h>
using namespace std;

// SOS (Sum Over Subsets) DP for subset enumeration
class SOSDP {
public:
    vector<long long> dp;
    int n;
    
    SOSDP(vector<long long>& arr) {
        n = arr.size();
        dp = arr;
        
        // SOS DP to compute sum over all subsets
        for (int i = 0; i < 20; i++) {
            for (int mask = 0; mask < (1 << 20); mask++) {
                if (mask & (1 << i)) {
                    dp[mask] += dp[mask ^ (1 << i)];
                }
            }
        }
    }
    
    long long getSubsetSum(int mask) {
        return dp[mask];
    }
    
    // Count subsets with XOR = target
    static int countXORSubsets(vector<int>& arr, int target) {
        int n = arr.size();
        vector<vector<int>> dp(n + 1, vector<int>(1024, 0));
        dp[0][0] = 1;
        
        for (int i = 0; i < n; i++) {
            for (int xor_val = 0; xor_val < 1024; xor_val++) {
                dp[i + 1][xor_val] = dp[i][xor_val]; // Don't take arr[i]
                dp[i + 1][xor_val ^ arr[i]] += dp[i][xor_val]; // Take arr[i]
            }
        }
        
        return dp[n][target];
    }
    
    // Fast Walsh-Hadamard Transform
    static void fwht(vector<long long>& a, bool invert = false) {
        int n = a.size();
        for (int len = 2; len <= n; len <<= 1) {
            for (int i = 0; i < n; i += len) {
                for (int j = 0; j < len / 2; j++) {
                    long long u = a[i + j], v = a[i + j + len / 2];
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                }
            }
        }
        if (invert) {
            for (long long& x : a) x /= n;
        }
    }
};

// Example usage:
/*
int main() {
    vector<long long> arr = {1, 2, 3, 4};
    SOSDP sos(arr);
    
    cout << "Subset sum for mask 3: " << sos.getSubsetSum(3) << endl;
    
    vector<int> xor_arr = {1, 2, 3};
    cout << "Subsets with XOR 0: " << SOSDP::countXORSubsets(xor_arr, 0) << endl;
    
    return 0;
}
*/