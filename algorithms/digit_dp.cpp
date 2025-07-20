#include <bits/stdc++.h>
using namespace std;

// Digit DP for counting numbers with constraints
class DigitDP {
public:
    string num;
    vector<vector<vector<int>>> dp;
    
    DigitDP(string s) : num(s) {
        int n = s.length();
        dp.assign(n, vector<vector<int>>(2, vector<int>(2, -1)));
    }
    
    // Count numbers <= num with given property
    int solve(int pos, bool tight, bool started) {
        if (pos == num.length()) {
            return started ? 1 : 0;
        }
        
        if (dp[pos][tight][started] != -1) {
            return dp[pos][tight][started];
        }
        
        int limit = tight ? (num[pos] - '0') : 9;
        int result = 0;
        
        for (int digit = 0; digit <= limit; digit++) {
            bool newTight = tight && (digit == limit);
            bool newStarted = started || (digit > 0);
            
            // Add your digit constraint here
            // Example: count numbers with no consecutive same digits
            if (!started || digit != (num[pos-1] - '0')) {
                result += solve(pos + 1, newTight, newStarted);
            }
        }
        
        return dp[pos][tight][started] = result;
    }
    
    int countNumbers() {
        return solve(0, true, false);
    }
    
    // Count numbers with sum of digits = target
    int countWithSum(int target) {
        dp.assign(num.length(), vector<vector<int>>(2, vector<int>(target + 1, -1)));
        return solveSum(0, true, false, 0, target);
    }
    
private:
    int solveSum(int pos, bool tight, bool started, int sum, int target) {
        if (pos == num.length()) {
            return (started && sum == target) ? 1 : 0;
        }
        
        if (sum > target) return 0;
        
        int limit = tight ? (num[pos] - '0') : 9;
        int result = 0;
        
        for (int digit = 0; digit <= limit; digit++) {
            bool newTight = tight && (digit == limit);
            bool newStarted = started || (digit > 0);
            
            result += solveSum(pos + 1, newTight, newStarted, sum + digit, target);
        }
        
        return result;
    }
};

// Example usage:
/*
int main() {
    DigitDP digitDP("123");
    cout << "Count: " << digitDP.countNumbers() << endl;
    cout << "Count with sum 5: " << digitDP.countWithSum(5) << endl;
    
    return 0;
}
*/