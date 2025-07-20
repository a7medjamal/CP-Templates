#include <bits/stdc++.h>
using namespace std;

// Dynamic Programming Algorithms
namespace DynamicProgramming {
    
    // Longest Increasing Subsequence (LIS)
    int longestIncreasingSubsequence(const vector<int>& arr) {
        if (arr.empty()) return 0;
        
        vector<int> lis;
        
        for (int num : arr) {
            auto it = lower_bound(lis.begin(), lis.end(), num);
            if (it == lis.end()) {
                lis.push_back(num);
            } else {
                *it = num;
            }
        }
        
        return lis.size();
    }
    
    // LIS with actual sequence
    vector<int> getLIS(const vector<int>& arr) {
        if (arr.empty()) return {};
        
        vector<int> lis;
        vector<int> parent(arr.size(), -1);
        vector<int> lisIndex;
        
        for (int i = 0; i < (int)arr.size(); i++) {
            auto it = lower_bound(lis.begin(), lis.end(), arr[i]);
            int pos = it - lis.begin();
            
            if (it == lis.end()) {
                lis.push_back(arr[i]);
                lisIndex.push_back(i);
            } else {
                *it = arr[i];
                lisIndex[pos] = i;
            }
            
            if (pos > 0) {
                parent[i] = lisIndex[pos - 1];
            }
        }
        
        // Reconstruct LIS
        vector<int> result;
        int curr = lisIndex.back();
        while (curr != -1) {
            result.push_back(arr[curr]);
            curr = parent[curr];
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
    
    // Longest Decreasing Subsequence
    int longestDecreasingSubsequence(const vector<int>& arr) {
        vector<int> reversed = arr;
        reverse(reversed.begin(), reversed.end());
        for (int& x : reversed) x = -x;
        return longestIncreasingSubsequence(reversed);
    }
    
    // Longest Common Subsequence (LCS)
    int longestCommonSubsequence(const string& s1, const string& s2) {
        int m = s1.length(), n = s2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        return dp[m][n];
    }
    
    // LCS with actual sequence
    string getLCS(const string& s1, const string& s2) {
        int m = s1.length(), n = s2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        // Reconstruct LCS
        string lcs = "";
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (s1[i - 1] == s2[j - 1]) {
                lcs = s1[i - 1] + lcs;
                i--; j--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
        
        return lcs;
    }
    
    // Edit Distance (Levenshtein Distance)
    int editDistance(const string& s1, const string& s2) {
        int m = s1.length(), n = s2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        
        // Initialize base cases
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
                }
            }
        }
        
        return dp[m][n];
    }
    
    // Longest Palindromic Subsequence
    int longestPalindromicSubsequence(const string& s) {
        int n = s.length();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // Every single character is a palindrome of length 1
        for (int i = 0; i < n; i++) {
            dp[i][i] = 1;
        }
        
        // Check for palindromes of length 2 and more
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                } else {
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        
        return dp[0][n - 1];
    }
    
    // Coin Change - Minimum coins
    int coinChange(const vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;
        
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (coin <= i) {
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        
        return dp[amount] > amount ? -1 : dp[amount];
    }
    
    // Coin Change - Number of ways
    int coinChangeWays(const vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1;
        
        for (int coin : coins) {
            for (int i = coin; i <= amount; i++) {
                dp[i] += dp[i - coin];
            }
        }
        
        return dp[amount];
    }
    
    // 0/1 Knapsack
    int knapsack01(const vector<int>& weights, const vector<int>& values, int capacity) {
        int n = weights.size();
        vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
        
        for (int i = 1; i <= n; i++) {
            for (int w = 1; w <= capacity; w++) {
                if (weights[i - 1] <= w) {
                    dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
                } else {
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }
        
        return dp[n][capacity];
    }
    
    // Unbounded Knapsack
    int knapsackUnbounded(const vector<int>& weights, const vector<int>& values, int capacity) {
        vector<int> dp(capacity + 1, 0);
        
        for (int i = 1; i <= capacity; i++) {
            for (int j = 0; j < (int)weights.size(); j++) {
                if (weights[j] <= i) {
                    dp[i] = max(dp[i], dp[i - weights[j]] + values[j]);
                }
            }
        }
        
        return dp[capacity];
    }
    
    // House Robber
    int rob(const vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        
        int prev2 = 0, prev1 = 0;
        for (int num : nums) {
            int curr = max(prev1, prev2 + num);
            prev2 = prev1;
            prev1 = curr;
        }
        
        return prev1;
    }
    
    // House Robber II (circular)
    int robCircular(const vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        if (nums.size() == 2) return max(nums[0], nums[1]);
        
        // Case 1: Rob houses 0 to n-2
        vector<int> case1(nums.begin(), nums.end() - 1);
        // Case 2: Rob houses 1 to n-1
        vector<int> case2(nums.begin() + 1, nums.end());
        
        return max(rob(case1), rob(case2));
    }
}

// Example usage:
/*
int main() {
    using namespace DynamicProgramming;
    
    vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    cout << "LIS length: " << longestIncreasingSubsequence(arr) << endl;
    
    vector<int> lis = getLIS(arr);
    cout << "LIS: ";
    for (int x : lis) cout << x << " ";
    cout << endl;
    
    string s1 = "ABCDGH", s2 = "AEDFHR";
    cout << "LCS length: " << longestCommonSubsequence(s1, s2) << endl;
    cout << "LCS: " << getLCS(s1, s2) << endl;
    cout << "Edit distance: " << editDistance(s1, s2) << endl;
    
    vector<int> coins = {1, 3, 4};
    cout << "Min coins for 6: " << coinChange(coins, 6) << endl;
    cout << "Ways to make 6: " << coinChangeWays(coins, 6) << endl;
    
    vector<int> weights = {1, 3, 4, 5};
    vector<int> values = {1, 4, 5, 7};
    cout << "0/1 Knapsack (capacity 7): " << knapsack01(weights, values, 7) << endl;
    
    return 0;
}
*/