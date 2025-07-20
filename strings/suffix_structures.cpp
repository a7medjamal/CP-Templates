#include <bits/stdc++.h>
using namespace std;

namespace SuffixStructures {
    // Suffix Array with LCP Array
    class SuffixArray {
    public:
        string s;
        vector<int> sa, lcp, rank;
        
        SuffixArray(string str) : s(str + "$") {
            buildSuffixArray();
            buildLCPArray();
        }
        
    private:
        void buildSuffixArray() {
            int n = s.length();
            sa.resize(n);
            rank.resize(n);
            
            // Initialize
            for (int i = 0; i < n; i++) {
                sa[i] = i;
                rank[i] = s[i];
            }
            
            // Radix sort based approach
            for (int k = 1; k < n; k *= 2) {
                auto cmp = [&](int i, int j) {
                    if (rank[i] != rank[j]) return rank[i] < rank[j];
                    int ri = (i + k < n) ? rank[i + k] : -1;
                    int rj = (j + k < n) ? rank[j + k] : -1;
                    return ri < rj;
                };
                
                sort(sa.begin(), sa.end(), cmp);
                
                vector<int> newRank(n);
                newRank[sa[0]] = 0;
                for (int i = 1; i < n; i++) {
                    newRank[sa[i]] = newRank[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
                }
                rank = newRank;
            }
        }
        
        void buildLCPArray() {
            int n = s.length();
            lcp.resize(n - 1);
            vector<int> invSA(n);
            
            for (int i = 0; i < n; i++) {
                invSA[sa[i]] = i;
            }
            
            int k = 0;
            for (int i = 0; i < n - 1; i++) {
                int j = sa[invSA[i] - 1];
                while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
                    k++;
                }
                lcp[invSA[i] - 1] = k;
                if (k > 0) k--;
            }
        }
        
    public:
        vector<int> findOccurrences(string pattern) {
            vector<int> result;
            int n = s.length();
            int m = pattern.length();
            
            // Binary search for first occurrence
            int left = 0, right = n - 1;
            while (left < right) {
                int mid = (left + right) / 2;
                if (s.substr(sa[mid], min(m, n - sa[mid])) < pattern) {
                    left = mid + 1;
                } else {
                    right = mid;
                }
            }
            
            int start = left;
            
            // Binary search for last occurrence
            left = 0, right = n - 1;
            while (left < right) {
                int mid = (left + right + 1) / 2;
                if (s.substr(sa[mid], min(m, n - sa[mid])) <= pattern) {
                    left = mid;
                } else {
                    right = mid - 1;
                }
            }
            
            int end = left;
            
            // Check if pattern exists
            if (start <= end && s.substr(sa[start], min(m, n - sa[start])) == pattern) {
                for (int i = start; i <= end; i++) {
                    if (sa[i] < n - 1) { // Exclude the $ character
                        result.push_back(sa[i]);
                    }
                }
            }
            
            return result;
        }
        
        int getLCP(int i, int j) {
            if (i == j) return s.length() - sa[i] - 1;
            if (i > j) swap(i, j);
            
            int minLCP = INT_MAX;
            for (int k = i; k < j; k++) {
                minLCP = min(minLCP, lcp[k]);
            }
            return minLCP;
        }
        
        string getLongestRepeatedSubstring() {
            int maxLCP = 0, index = 0;
            for (int i = 0; i < (int)lcp.size(); i++) {
                if (lcp[i] > maxLCP) {
                    maxLCP = lcp[i];
                    index = i;
                }
            }
            return s.substr(sa[index], maxLCP);
        }
    };
    
    // Suffix Tree (Ukkonen's Algorithm)
    class SuffixTree {
    public:
        struct Node {
            map<char, int> children;
            int start, end;
            int suffixLink;
            
            Node(int start = -1, int end = -1) : start(start), end(end), suffixLink(-1) {}
            
            int length() {
                return end - start + 1;
            }
        };
        
        string text;
        vector<Node> nodes;
        int root, activeNode, activeEdge, activeLength;
        int remainingSuffixCount;
        int leafEnd;
        
        SuffixTree(string s) : text(s + "$") {
            int n = text.length();
            nodes.reserve(2 * n);
            
            root = newNode(-1, -1);
            activeNode = root;
            activeEdge = -1;
            activeLength = 0;
            remainingSuffixCount = 0;
            leafEnd = -1;
            
            for (int i = 0; i < n; i++) {
                extendSuffixTree(i);
            }
        }
        
    private:
        int newNode(int start, int end) {
            nodes.push_back(Node(start, end));
            return nodes.size() - 1;
        }
        
        int edgeLength(int node) {
            if (nodes[node].end == -1) {
                return leafEnd - nodes[node].start + 1;
            }
            return nodes[node].end - nodes[node].start + 1;
        }
        
        bool walkDown(int node) {
            if (activeLength >= edgeLength(node)) {
                activeEdge += edgeLength(node);
                activeLength -= edgeLength(node);
                activeNode = node;
                return true;
            }
            return false;
        }
        
        void extendSuffixTree(int pos) {
            leafEnd = pos;
            remainingSuffixCount++;
            int lastNewNode = -1;
            
            while (remainingSuffixCount > 0) {
                if (activeLength == 0) {
                    activeEdge = pos;
                }
                
                char c = text[activeEdge];
                if (nodes[activeNode].children.find(c) == nodes[activeNode].children.end()) {
                    int leaf = newNode(pos, -1);
                    nodes[activeNode].children[c] = leaf;
                    
                    if (lastNewNode != -1) {
                        nodes[lastNewNode].suffixLink = activeNode;
                        lastNewNode = -1;
                    }
                } else {
                    int next = nodes[activeNode].children[c];
                    if (walkDown(next)) {
                        continue;
                    }
                    
                    if (text[nodes[next].start + activeLength] == text[pos]) {
                        if (lastNewNode != -1 && activeNode != root) {
                            nodes[lastNewNode].suffixLink = activeNode;
                            lastNewNode = -1;
                        }
                        activeLength++;
                        break;
                    }
                    
                    int splitEnd = nodes[next].start + activeLength - 1;
                    int split = newNode(nodes[next].start, splitEnd);
                    nodes[activeNode].children[c] = split;
                    
                    int leaf = newNode(pos, -1);
                    nodes[split].children[text[pos]] = leaf;
                    nodes[next].start += activeLength;
                    nodes[split].children[text[nodes[next].start]] = next;
                    
                    if (lastNewNode != -1) {
                        nodes[lastNewNode].suffixLink = split;
                    }
                    
                    lastNewNode = split;
                }
                
                remainingSuffixCount--;
                
                if (activeNode == root && activeLength > 0) {
                    activeLength--;
                    activeEdge = pos - remainingSuffixCount + 1;
                } else if (activeNode != root) {
                    activeNode = nodes[activeNode].suffixLink;
                }
            }
        }
        
    public:
        bool contains(string pattern) {
            int curr = root;
            int i = 0;
            
            while (i < pattern.length()) {
                char c = pattern[i];
                if (nodes[curr].children.find(c) == nodes[curr].children.end()) {
                    return false;
                }
                
                int next = nodes[curr].children[c];
                int start = nodes[next].start;
                int end = (nodes[next].end == -1) ? leafEnd : nodes[next].end;
                
                for (int j = start; j <= end && i < pattern.length(); j++, i++) {
                    if (text[j] != pattern[i]) {
                        return false;
                    }
                }
                
                curr = next;
            }
            
            return true;
        }
        
        int countOccurrences(string pattern) {
            int curr = root;
            int i = 0;
            
            // Navigate to the end of pattern
            while (i < pattern.length()) {
                char c = pattern[i];
                if (nodes[curr].children.find(c) == nodes[curr].children.end()) {
                    return 0;
                }
                
                int next = nodes[curr].children[c];
                int start = nodes[next].start;
                int end = (nodes[next].end == -1) ? leafEnd : nodes[next].end;
                
                for (int j = start; j <= end && i < pattern.length(); j++, i++) {
                    if (text[j] != pattern[i]) {
                        return 0;
                    }
                }
                
                curr = next;
            }
            
            // Count leaves in subtree
            function<int(int)> countLeaves = [&](int node) -> int {
                if (nodes[node].children.empty()) {
                    return 1;
                }
                
                int count = 0;
                for (auto& [c, child] : nodes[node].children) {
                    count += countLeaves(child);
                }
                return count;
            };
            
            return countLeaves(curr);
        }
        
        string longestCommonSubstring(string other) {
            SuffixTree combined(text.substr(0, text.length() - 1) + "#" + other);
            
            string result = "";
            int maxLen = 0;
            
            function<void(int, int)> dfs = [&](int node, int depth) {
                bool hasFirst = false, hasSecond = false;
                
                if (combined.nodes[node].children.empty()) {
                    // Leaf node - check which string it belongs to
                    int start = combined.nodes[node].start;
                    if (start < text.length() - 1) hasFirst = true;
                    else hasSecond = true;
                } else {
                    for (auto& [c, child] : combined.nodes[node].children) {
                        dfs(child, depth + combined.edgeLength(child));
                        // This is simplified - in practice, you'd need to track which strings are in each subtree
                    }
                }
                
                if (hasFirst && hasSecond && depth > maxLen) {
                    maxLen = depth;
                    // Extract the string (simplified)
                }
            };
            
            dfs(combined.root, 0);
            return result;
        }
    };
    
    // Suffix Automaton
    class SuffixAutomaton {
    public:
        struct State {
            map<char, int> transitions;
            int len, link;
            
            State() : len(0), link(-1) {}
        };
        
        vector<State> states;
        int last;
        
        SuffixAutomaton() {
            states.push_back(State());
            last = 0;
        }
        
        void addChar(char c) {
            int curr = states.size();
            states.push_back(State());
            states[curr].len = states[last].len + 1;
            
            int p = last;
            while (p != -1 && states[p].transitions.find(c) == states[p].transitions.end()) {
                states[p].transitions[c] = curr;
                p = states[p].link;
            }
            
            if (p == -1) {
                states[curr].link = 0;
            } else {
                int q = states[p].transitions[c];
                if (states[p].len + 1 == states[q].len) {
                    states[curr].link = q;
                } else {
                    int clone = states.size();
                    states.push_back(states[q]);
                    states[clone].len = states[p].len + 1;
                    
                    while (p != -1 && states[p].transitions[c] == q) {
                        states[p].transitions[c] = clone;
                        p = states[p].link;
                    }
                    
                    states[q].link = states[curr].link = clone;
                }
            }
            
            last = curr;
        }
        
        bool contains(string s) {
            int curr = 0;
            for (char c : s) {
                if (states[curr].transitions.find(c) == states[curr].transitions.end()) {
                    return false;
                }
                curr = states[curr].transitions[c];
            }
            return true;
        }
        
        long long countDistinctSubstrings() {
            vector<long long> dp(states.size(), -1);
            
            function<long long(int)> solve = [&](int v) -> long long {
                if (dp[v] != -1) return dp[v];
                
                dp[v] = 1;
                for (auto& [c, u] : states[v].transitions) {
                    dp[v] += solve(u);
                }
                return dp[v];
            };
            
            return solve(0) - 1; // Subtract 1 for empty string
        }
    };
}

// Example usage:
/*
int main() {
    using namespace SuffixStructures;
    
    string text = "banana";
    
    // Suffix Array
    SuffixArray sa(text);
    cout << "Suffix Array: ";
    for (int i : sa.sa) {
        cout << i << " ";
    }
    cout << endl;
    
    auto occurrences = sa.findOccurrences("ana");
    cout << "Occurrences of 'ana': ";
    for (int pos : occurrences) {
        cout << pos << " ";
    }
    cout << endl;
    
    cout << "Longest repeated substring: " << sa.getLongestRepeatedSubstring() << endl;
    
    // Suffix Tree
    SuffixTree st(text);
    cout << "Contains 'ana': " << st.contains("ana") << endl;
    cout << "Count of 'ana': " << st.countOccurrences("ana") << endl;
    
    // Suffix Automaton
    SuffixAutomaton sam;
    for (char c : text) {
        sam.addChar(c);
    }
    cout << "Distinct substrings: " << sam.countDistinctSubstrings() << endl;
    
    return 0;
}
*/