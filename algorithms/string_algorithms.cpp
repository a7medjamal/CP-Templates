#include <bits/stdc++.h>
using namespace std;

// String Processing Algorithms
namespace StringAlgorithms {
    
    // Longest Palindromic Substring using Manacher's Algorithm
    string longestPalindromicSubstring(const string& s) {
        if (s.empty()) return "";
        
        // Transform string: "abc" -> "#a#b#c#"
        string T = "#";
        for (char c : s) {
            T += c;
            T += "#";
        }
        
        int n = T.length();
        vector<int> P(n, 0);
        int center = 0, right = 0;
        
        for (int i = 0; i < n; i++) {
            int mirror = 2 * center - i;
            
            if (i < right) {
                P[i] = min(right - i, P[mirror]);
            }
            
            // Try to expand palindrome centered at i
            while (i + P[i] + 1 < n && i - P[i] - 1 >= 0 && 
                   T[i + P[i] + 1] == T[i - P[i] - 1]) {
                P[i]++;
            }
            
            // If palindrome centered at i extends past right, adjust center and right
            if (i + P[i] > right) {
                center = i;
                right = i + P[i];
            }
        }
        
        // Find the longest palindrome
        int maxLen = 0, centerIndex = 0;
        for (int i = 0; i < n; i++) {
            if (P[i] > maxLen) {
                maxLen = P[i];
                centerIndex = i;
            }
        }
        
        int start = (centerIndex - maxLen) / 2;
        return s.substr(start, maxLen);
    }
    
    // All Palindromic Substrings
    vector<string> getAllPalindromes(const string& s) {
        vector<string> palindromes;
        int n = s.length();
        
        // Check for odd length palindromes
        for (int center = 0; center < n; center++) {
            int left = center, right = center;
            while (left >= 0 && right < n && s[left] == s[right]) {
                palindromes.push_back(s.substr(left, right - left + 1));
                left--;
                right++;
            }
        }
        
        // Check for even length palindromes
        for (int center = 0; center < n - 1; center++) {
            int left = center, right = center + 1;
            while (left >= 0 && right < n && s[left] == s[right]) {
                palindromes.push_back(s.substr(left, right - left + 1));
                left--;
                right++;
            }
        }
        
        return palindromes;
    }
    
    // Count Palindromic Substrings
    int countPalindromes(const string& s) {
        int count = 0;
        int n = s.length();
        
        // Check for odd length palindromes
        for (int center = 0; center < n; center++) {
            int left = center, right = center;
            while (left >= 0 && right < n && s[left] == s[right]) {
                count++;
                left--;
                right++;
            }
        }
        
        // Check for even length palindromes
        for (int center = 0; center < n - 1; center++) {
            int left = center, right = center + 1;
            while (left >= 0 && right < n && s[left] == s[right]) {
                count++;
                left--;
                right++;
            }
        }
        
        return count;
    }
    
    // Longest Palindromic Subsequence (Dynamic Programming)
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
    
    // Check if string is palindrome
    bool isPalindrome(const string& s) {
        int left = 0, right = s.length() - 1;
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
    
    // Valid Palindrome (ignoring non-alphanumeric)
    bool isValidPalindrome(const string& s) {
        int left = 0, right = s.length() - 1;
        
        while (left < right) {
            while (left < right && !isalnum(s[left])) left++;
            while (left < right && !isalnum(s[right])) right--;
            
            if (tolower(s[left]) != tolower(s[right])) return false;
            left++;
            right--;
        }
        
        return true;
    }
    
    // Minimum insertions to make palindrome
    int minInsertionsPalindrome(const string& s) {
        int n = s.length();
        string rev = s;
        reverse(rev.begin(), rev.end());
        
        // Find LCS of s and reverse of s
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i - 1] == rev[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        return n - dp[n][n];
    }
    
    // Palindrome Partitioning - Check if partition is possible
    bool canPartitionPalindrome(const string& s, int k) {
        int n = s.length();
        if (k > n) return false;
        if (k == n) return true;
        
        // Precompute palindrome table
        vector<vector<bool>> isPalin(n, vector<bool>(n, false));
        
        for (int i = 0; i < n; i++) {
            isPalin[i][i] = true;
        }
        
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    isPalin[i][j] = (len == 2) || isPalin[i + 1][j - 1];
                }
            }
        }
        
        // DP: dp[i][j] = minimum cuts needed for first i characters using j partitions
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, INT_MAX));
        dp[0][0] = 0;
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= min(i, k); j++) {
                for (int prev = j - 1; prev < i; prev++) {
                    if (dp[prev][j - 1] != INT_MAX && isPalin[prev][i - 1]) {
                        dp[i][j] = min(dp[i][j], dp[prev][j - 1] + 1);
                    }
                }
            }
        }
        
        return dp[n][k] == k;
    }
    
    // Minimum cuts for palindrome partitioning
    int minCutsPalindromePartition(const string& s) {
        int n = s.length();
        vector<vector<bool>> isPalin(n, vector<bool>(n, false));
        
        // Precompute palindrome table
        for (int i = 0; i < n; i++) {
            isPalin[i][i] = true;
        }
        
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    isPalin[i][j] = (len == 2) || isPalin[i + 1][j - 1];
                }
            }
        }
        
        vector<int> cuts(n, INT_MAX);
        for (int i = 0; i < n; i++) {
            if (isPalin[0][i]) {
                cuts[i] = 0;
            } else {
                for (int j = 0; j < i; j++) {
                    if (isPalin[j + 1][i]) {
                        cuts[i] = min(cuts[i], cuts[j] + 1);
                    }
                }
            }
        }
        
        return cuts[n - 1];
    }
    
    // Expand around center helper
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left--;
            right++;
        }
        return right - left - 1;
    }
    
    // Longest palindrome using expand around center
    string longestPalindromeExpand(const string& s) {
        if (s.empty()) return "";
        
        int start = 0, maxLen = 1;
        
        for (int i = 0; i < s.length(); i++) {
            // Odd length palindromes
            int len1 = expandAroundCenter(s, i, i);
            // Even length palindromes
            int len2 = expandAroundCenter(s, i, i + 1);
            
            int len = max(len1, len2);
            if (len > maxLen) {
                maxLen = len;
                start = i - (len - 1) / 2;
            }
        }
        
        return s.substr(start, maxLen);
    }
    
    // String reversal
    string reverseString(string s) {
        reverse(s.begin(), s.end());
        return s;
    }
    
    // Reverse words in string
    string reverseWords(string s) {
        // Remove extra spaces
        int i = 0, j = 0;
        while (j < s.length()) {
            while (j < s.length() && s[j] == ' ') j++;
            while (j < s.length() && s[j] != ' ') s[i++] = s[j++];
            while (j < s.length() && s[j] == ' ') j++;
            if (j < s.length()) s[i++] = ' ';
        }
        s.resize(i);
        
        // Reverse entire string
        reverse(s.begin(), s.end());
        
        // Reverse each word
        int start = 0;
        for (int end = 0; end <= s.length(); end++) {
            if (end == s.length() || s[end] == ' ') {
                reverse(s.begin() + start, s.begin() + end);
                start = end + 1;
            }
        }
        
        return s;
    }
}

// Example usage:
/*
int main() {
    using namespace StringAlgorithms;
    
    string s = "babad";
    cout << "Longest palindromic substring: " << longestPalindromicSubstring(s) << endl;
    cout << "Longest palindromic subsequence length: " << longestPalindromicSubsequence(s) << endl;
    cout << "Count of palindromic substrings: " << countPalindromes(s) << endl;
    
    cout << "Is palindrome: " << isPalindrome("racecar") << endl;
    cout << "Is valid palindrome: " << isValidPalindrome("A man, a plan, a canal: Panama") << endl;
    
    cout << "Min insertions for palindrome: " << minInsertionsPalindrome("abc") << endl;
    cout << "Min cuts for palindrome partition: " << minCutsPalindromePartition("aab") << endl;
    
    cout << "Reverse words: " << reverseWords("the sky is blue") << endl;
    
    return 0;
}
*/