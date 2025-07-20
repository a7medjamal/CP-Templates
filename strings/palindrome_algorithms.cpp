#include <bits/stdc++.h>
using namespace std;

namespace PalindromeAlgorithms {
    // Manacher's Algorithm for finding all palindromes
    class Manacher {
    public:
        static vector<int> manacher(string s) {
            string t = "#";
            for (char c : s) {
                t += c;
                t += "#";
            }
            
            int n = t.length();
            vector<int> p(n, 0);
            int center = 0, right = 0;
            
            for (int i = 0; i < n; i++) {
                if (i < right) {
                    p[i] = min(right - i, p[2 * center - i]);
                }
                
                while (i + p[i] + 1 < n && i - p[i] - 1 >= 0 && 
                       t[i + p[i] + 1] == t[i - p[i] - 1]) {
                    p[i]++;
                }
                
                if (i + p[i] > right) {
                    center = i;
                    right = i + p[i];
                }
            }
            
            return p;
        }
        
        static string longestPalindrome(string s) {
            if (s.empty()) return "";
            
            vector<int> p = manacher(s);
            int maxLen = 0, centerIndex = 0;
            
            for (int i = 0; i < p.size(); i++) {
                if (p[i] > maxLen) {
                    maxLen = p[i];
                    centerIndex = i;
                }
            }
            
            int start = (centerIndex - maxLen) / 2;
            return s.substr(start, maxLen);
        }
        
        static vector<pair<int, int>> getAllPalindromes(string s) {
            vector<pair<int, int>> palindromes;
            vector<int> p = manacher(s);
            
            for (int i = 0; i < p.size(); i++) {
                if (p[i] > 0) {
                    int start = (i - p[i]) / 2;
                    int length = p[i];
                    palindromes.push_back({start, start + length - 1});
                }
            }
            
            return palindromes;
        }
        
        static bool isPalindrome(string s, int l, int r) {
            vector<int> p = manacher(s);
            int center = l + r + 1;
            int radius = r - l + 1;
            return p[center] >= radius;
        }
    };
    
    // Palindromic Tree (Eertree)
    class PalindromicTree {
    public:
        struct Node {
            map<char, int> children;
            int len, suffixLink;
            int count; // number of times this palindrome appears
            
            Node(int len = 0) : len(len), suffixLink(-1), count(0) {}
        };
        
        string s;
        vector<Node> tree;
        int lastNode;
        
        PalindromicTree() {
            tree.push_back(Node(-1)); // odd root
            tree.push_back(Node(0));  // even root
            tree[0].suffixLink = 0;
            tree[1].suffixLink = 0;
            lastNode = 1;
        }
        
        void addChar(char c) {
            s += c;
            int pos = s.length() - 1;
            int curr = lastNode;
            
            // Find the longest palindromic suffix
            while (true) {
                int len = tree[curr].len;
                if (pos - len - 1 >= 0 && s[pos - len - 1] == c) {
                    break;
                }
                curr = tree[curr].suffixLink;
            }
            
            if (tree[curr].children.find(c) != tree[curr].children.end()) {
                lastNode = tree[curr].children[c];
                tree[lastNode].count++;
                return;
            }
            
            // Create new node
            int newNode = tree.size();
            tree.push_back(Node(tree[curr].len + 2));
            tree[curr].children[c] = newNode;
            tree[newNode].count = 1;
            
            if (tree[newNode].len == 1) {
                tree[newNode].suffixLink = 1; // even root
            } else {
                curr = tree[curr].suffixLink;
                while (true) {
                    int len = tree[curr].len;
                    if (pos - len - 1 >= 0 && s[pos - len - 1] == c) {
                        tree[newNode].suffixLink = tree[curr].children[c];
                        break;
                    }
                    curr = tree[curr].suffixLink;
                }
            }
            
            lastNode = newNode;
        }
        
        int countDistinctPalindromes() {
            return tree.size() - 2; // Exclude the two roots
        }
        
        long long countAllPalindromes() {
            // Calculate suffix link counts
            for (int i = tree.size() - 1; i >= 2; i--) {
                tree[tree[i].suffixLink].count += tree[i].count;
            }
            
            long long total = 0;
            for (int i = 2; i < tree.size(); i++) {
                total += tree[i].count;
            }
            return total;
        }
        
        vector<pair<int, int>> getAllPalindromes() {
            vector<pair<int, int>> palindromes;
            for (int i = 2; i < tree.size(); i++) {
                palindromes.push_back({tree[i].len, tree[i].count});
            }
            return palindromes;
        }
        
        string getLongestPalindrome() {
            int maxLen = 0;
            int maxNode = -1;
            for (int i = 2; i < tree.size(); i++) {
                if (tree[i].len > maxLen) {
                    maxLen = tree[i].len;
                    maxNode = i;
                }
            }
            
            if (maxNode == -1) return "";
            
            // Reconstruct the palindrome
            string result;
            int curr = maxNode;
            while (tree[curr].len > 0) {
                // Find the character that leads to this node
                for (auto& [ch, child] : tree[tree[curr].suffixLink].children) {
                    if (child == curr) {
                        result = ch + result + ch;
                        break;
                    }
                }
                curr = tree[curr].suffixLink;
            }
            
            return result;
        }
    };
    
    // Simple palindrome checking functions
    bool isPalindrome(string s) {
        int n = s.length();
        for (int i = 0; i < n / 2; i++) {
            if (s[i] != s[n - 1 - i]) {
                return false;
            }
        }
        return true;
    }
    
    bool isPalindrome(string s, int l, int r) {
        while (l < r) {
            if (s[l] != s[r]) {
                return false;
            }
            l++;
            r--;
        }
        return true;
    }
    
    // Expand around center approach
    string longestPalindromeExpand(string s) {
        if (s.empty()) return "";
        
        int start = 0, maxLen = 1;
        
        auto expandAroundCenter = [&](int left, int right) {
            while (left >= 0 && right < s.length() && s[left] == s[right]) {
                int currentLen = right - left + 1;
                if (currentLen > maxLen) {
                    start = left;
                    maxLen = currentLen;
                }
                left--;
                right++;
            }
        };
        
        for (int i = 0; i < (int)s.length(); i++) {
            expandAroundCenter(i, i);     // odd length palindromes
            expandAroundCenter(i, i + 1); // even length palindromes
        }
        
        return s.substr(start, maxLen);
    }
    
    // Count palindromic substrings
    int countPalindromicSubstrings(string s) {
        int count = 0;
        
        auto expandAroundCenter = [&](int left, int right) {
            while (left >= 0 && right < s.length() && s[left] == s[right]) {
                count++;
                left--;
                right++;
            }
        };
        
        for (int i = 0; i < s.length(); i++) {
            expandAroundCenter(i, i);     // odd length palindromes
            expandAroundCenter(i, i + 1); // even length palindromes
        }
        
        return count;
    }
    
    // Minimum insertions to make palindrome
    int minInsertionsToMakePalindrome(string s) {
        int n = s.length();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    dp[i][j] = dp[i + 1][j - 1];
                } else {
                    dp[i][j] = 1 + min(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        
        return dp[0][n - 1];
    }
    
    // Palindromic partitioning
    vector<vector<string>> palindromePartition(string s) {
        vector<vector<string>> result;
        vector<string> current;
        
        function<void(int)> backtrack = [&](int start) {
            if (start == s.length()) {
                result.push_back(current);
                return;
            }
            
            for (int end = start; end < s.length(); end++) {
                if (isPalindrome(s, start, end)) {
                    current.push_back(s.substr(start, end - start + 1));
                    backtrack(end + 1);
                    current.pop_back();
                }
            }
        };
        
        backtrack(0);
        return result;
    }
    
    // Minimum cuts for palindrome partitioning
    int minCutsPalindromePartition(string s) {
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
}

// Example usage:
/*
int main() {
    using namespace PalindromeAlgorithms;
    
    string text = "aabaa";
    
    // Manacher's algorithm
    cout << "Longest palindrome: " << Manacher::longestPalindrome(text) << endl;
    
    auto palindromes = Manacher::getAllPalindromes(text);
    cout << "All palindromes: ";
    for (auto [start, end] : palindromes) {
        cout << "[" << start << ", " << end << "] ";
    }
    cout << endl;
    
    // Palindromic Tree
    PalindromicTree pt;
    for (char c : text) {
        pt.addChar(c);
    }
    cout << "Distinct palindromes: " << pt.countDistinctPalindromes() << endl;
    cout << "Total palindromes: " << pt.countAllPalindromes() << endl;
    
    // Simple functions
    cout << "Palindromic substrings count: " << countPalindromicSubstrings(text) << endl;
    
    return 0;
}
*/