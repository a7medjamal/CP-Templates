#include <bits/stdc++.h>
using namespace std;

namespace PatternMatching {
    // KMP (Knuth-Morris-Pratt) Algorithm
    vector<int> computeLPS(string pattern) {
        int m = pattern.length();
        vector<int> lps(m, 0);
        int len = 0;
        int i = 1;
        
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
        
        return lps;
    }
    
    vector<int> KMPSearch(string text, string pattern) {
        vector<int> result;
        int n = text.length();
        int m = pattern.length();
        
        if (m == 0) return result;
        
        vector<int> lps = computeLPS(pattern);
        
        int i = 0; // index for text
        int j = 0; // index for pattern
        
        while (i < n) {
            if (pattern[j] == text[i]) {
                i++;
                j++;
            }
            
            if (j == m) {
                result.push_back(i - j);
                j = lps[j - 1];
            } else if (i < n && pattern[j] != text[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
        
        return result;
    }
    
    // Z Algorithm
    vector<int> zFunction(string s) {
        int n = s.length();
        vector<int> z(n);
        
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (i <= r) {
                z[i] = min(r - i + 1, z[i - l]);
            }
            
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                z[i]++;
            }
            
            if (i + z[i] - 1 > r) {
                l = i;
                r = i + z[i] - 1;
            }
        }
        
        return z;
    }
    
    vector<int> zSearch(string text, string pattern) {
        string combined = pattern + "#" + text;
        vector<int> z = zFunction(combined);
        vector<int> result;
        
        int patternLen = pattern.length();
        for (int i = patternLen + 1; i < combined.length(); i++) {
            if (z[i] == patternLen) {
                result.push_back(i - patternLen - 1);
            }
        }
        
        return result;
    }
    
    // Rabin-Karp Algorithm with Rolling Hash
    class RabinKarp {
    private:
        static const int MOD = 1e9 + 7;
        static const int BASE = 31;
        
    public:
        static vector<int> search(string text, string pattern) {
            vector<int> result;
            int n = text.length();
            int m = pattern.length();
            
            if (m > n) return result;
            
            long long patternHash = 0;
            long long textHash = 0;
            long long h = 1;
            
            // Calculate h = pow(BASE, m-1) % MOD
            for (int i = 0; i < m - 1; i++) {
                h = (h * BASE) % MOD;
            }
            
            // Calculate hash of pattern and first window of text
            for (int i = 0; i < m; i++) {
                patternHash = (patternHash * BASE + pattern[i]) % MOD;
                textHash = (textHash * BASE + text[i]) % MOD;
            }
            
            // Slide the pattern over text one by one
            for (int i = 0; i <= n - m; i++) {
                if (patternHash == textHash) {
                    // Check for exact match
                    bool match = true;
                    for (int j = 0; j < m; j++) {
                        if (text[i + j] != pattern[j]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        result.push_back(i);
                    }
                }
                
                // Calculate hash for next window
                if (i < n - m) {
                    textHash = (BASE * (textHash - text[i] * h) + text[i + m]) % MOD;
                    if (textHash < 0) textHash += MOD;
                }
            }
            
            return result;
        }
    };
    
    // Aho-Corasick Algorithm for Multiple Pattern Matching
    class AhoCorasick {
    public:
        struct Node {
            map<char, int> children;
            vector<int> output;
            int fail;
            
            Node() : fail(0) {}
        };
        
        vector<Node> trie;
        vector<string> patterns;
        
        AhoCorasick() {
            trie.push_back(Node());
        }
        
        void addPattern(string pattern) {
            int curr = 0;
            for (char c : pattern) {
                if (trie[curr].children.find(c) == trie[curr].children.end()) {
                    trie[curr].children[c] = trie.size();
                    trie.push_back(Node());
                }
                curr = trie[curr].children[c];
            }
            trie[curr].output.push_back(patterns.size());
            patterns.push_back(pattern);
        }
        
        void buildFailureFunction() {
            queue<int> q;
            
            // Initialize failure function for depth 1 nodes
            for (auto& [c, child] : trie[0].children) {
                trie[child].fail = 0;
                q.push(child);
            }
            
            while (!q.empty()) {
                int curr = q.front();
                q.pop();
                
                for (auto& [c, child] : trie[curr].children) {
                    q.push(child);
                    
                    int fail = trie[curr].fail;
                    while (fail != 0 && trie[fail].children.find(c) == trie[fail].children.end()) {
                        fail = trie[fail].fail;
                    }
                    
                    if (trie[fail].children.find(c) != trie[fail].children.end() && trie[fail].children[c] != child) {
                        trie[child].fail = trie[fail].children[c];
                    } else {
                        trie[child].fail = 0;
                    }
                    
                    // Copy output from failure link
                    for (int pattern_id : trie[trie[child].fail].output) {
                        trie[child].output.push_back(pattern_id);
                    }
                }
            }
        }
        
        vector<pair<int, int>> search(string text) {
            vector<pair<int, int>> matches;
            int curr = 0;
            
            for (int i = 0; i < text.length(); i++) {
                char c = text[i];
                
                while (curr != 0 && trie[curr].children.find(c) == trie[curr].children.end()) {
                    curr = trie[curr].fail;
                }
                
                if (trie[curr].children.find(c) != trie[curr].children.end()) {
                    curr = trie[curr].children[c];
                }
                
                for (int pattern_id : trie[curr].output) {
                    matches.push_back({i - patterns[pattern_id].length() + 1, pattern_id});
                }
            }
            
            return matches;
        }
    };
    
    // Boyer-Moore Algorithm (simplified version)
    class BoyerMoore {
    private:
        static vector<int> buildBadCharTable(string pattern) {
            vector<int> badChar(256, -1);
            for (int i = 0; i < pattern.length(); i++) {
                badChar[(int)pattern[i]] = i;
            }
            return badChar;
        }
        
    public:
        static vector<int> search(string text, string pattern) {
            vector<int> result;
            int n = text.length();
            int m = pattern.length();
            
            if (m > n) return result;
            
            vector<int> badChar = buildBadCharTable(pattern);
            
            int shift = 0;
            while (shift <= n - m) {
                int j = m - 1;
                
                while (j >= 0 && pattern[j] == text[shift + j]) {
                    j--;
                }
                
                if (j < 0) {
                    result.push_back(shift);
                    shift += (shift + m < n) ? m - badChar[text[shift + m]] : 1;
                } else {
                    shift += max(1, j - badChar[text[shift + j]]);
                }
            }
            
            return result;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace PatternMatching;
    
    string text = "ABABDABACDABABCABCABCABCABC";
    string pattern = "ABABCABCABCABC";
    
    // KMP Search
    auto kmp_result = KMPSearch(text, pattern);
    cout << "KMP matches at: ";
    for (int pos : kmp_result) {
        cout << pos << " ";
    }
    cout << endl;
    
    // Z Algorithm Search
    auto z_result = zSearch(text, pattern);
    cout << "Z Algorithm matches at: ";
    for (int pos : z_result) {
        cout << pos << " ";
    }
    cout << endl;
    
    // Rabin-Karp Search
    auto rk_result = RabinKarp::search(text, pattern);
    cout << "Rabin-Karp matches at: ";
    for (int pos : rk_result) {
        cout << pos << " ";
    }
    cout << endl;
    
    return 0;
}
*/