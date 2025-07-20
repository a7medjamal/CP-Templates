#include <bits/stdc++.h>
using namespace std;

namespace TrieStructures {
    // Basic Trie
    class Trie {
    public:
        struct TrieNode {
            map<char, TrieNode*> children;
            bool isEndOfWord;
            int count; // Number of words ending at this node
            int prefixCount; // Number of words with this prefix
            
            TrieNode() : isEndOfWord(false), count(0), prefixCount(0) {}
        };
        
        TrieNode* root;
        
        Trie() {
            root = new TrieNode();
        }
        
        void insert(string word) {
            TrieNode* curr = root;
            for (char c : word) {
                if (curr->children.find(c) == curr->children.end()) {
                    curr->children[c] = new TrieNode();
                }
                curr = curr->children[c];
                curr->prefixCount++;
            }
            curr->isEndOfWord = true;
            curr->count++;
        }
        
        bool search(string word) {
            TrieNode* curr = root;
            for (char c : word) {
                if (curr->children.find(c) == curr->children.end()) {
                    return false;
                }
                curr = curr->children[c];
            }
            return curr->isEndOfWord;
        }
        
        bool startsWith(string prefix) {
            TrieNode* curr = root;
            for (char c : prefix) {
                if (curr->children.find(c) == curr->children.end()) {
                    return false;
                }
                curr = curr->children[c];
            }
            return true;
        }
        
        int countWordsWithPrefix(string prefix) {
            TrieNode* curr = root;
            for (char c : prefix) {
                if (curr->children.find(c) == curr->children.end()) {
                    return 0;
                }
                curr = curr->children[c];
            }
            return curr->prefixCount;
        }
        
        void remove(string word) {
            function<bool(TrieNode*, string, int)> removeHelper = [&](TrieNode* node, string word, int index) -> bool {
                if (index == word.length()) {
                    if (!node->isEndOfWord) return false;
                    node->isEndOfWord = false;
                    node->count--;
                    return node->children.empty() && node->count == 0;
                }
                
                char c = word[index];
                if (node->children.find(c) == node->children.end()) {
                    return false;
                }
                
                TrieNode* child = node->children[c];
                child->prefixCount--;
                
                bool shouldDeleteChild = removeHelper(child, word, index + 1);
                
                if (shouldDeleteChild) {
                    delete child;
                    node->children.erase(c);
                    return !node->isEndOfWord && node->children.empty() && node->count == 0;
                }
                
                return false;
            };
            
            removeHelper(root, word, 0);
        }
        
        vector<string> getAllWords() {
            vector<string> result;
            string current = "";
            
            function<void(TrieNode*, string)> dfs = [&](TrieNode* node, string current) {
                if (node->isEndOfWord) {
                    for (int i = 0; i < node->count; i++) {
                        result.push_back(current);
                    }
                }
                
                for (auto& [c, child] : node->children) {
                    dfs(child, current + c);
                }
            };
            
            dfs(root, current);
            return result;
        }
        
        vector<string> getWordsWithPrefix(string prefix) {
            vector<string> result;
            TrieNode* curr = root;
            
            // Navigate to prefix
            for (char c : prefix) {
                if (curr->children.find(c) == curr->children.end()) {
                    return result;
                }
                curr = curr->children[c];
            }
            
            // DFS from prefix node
            function<void(TrieNode*, string)> dfs = [&](TrieNode* node, string current) {
                if (node->isEndOfWord) {
                    for (int i = 0; i < node->count; i++) {
                        result.push_back(current);
                    }
                }
                
                for (auto& [c, child] : node->children) {
                    dfs(child, current + c);
                }
            };
            
            dfs(curr, prefix);
            return result;
        }
        
        string longestCommonPrefix() {
            string result = "";
            TrieNode* curr = root;
            
            while (curr->children.size() == 1 && !curr->isEndOfWord) {
                auto it = curr->children.begin();
                result += it->first;
                curr = it->second;
            }
            
            return result;
        }
    };
    
    // Binary Trie for XOR operations
    class BinaryTrie {
    public:
        struct TrieNode {
            TrieNode* children[2];
            int count;
            
            TrieNode() : count(0) {
                children[0] = children[1] = nullptr;
            }
        };
        
        TrieNode* root;
        int maxBits;
        
        BinaryTrie(int bits = 30) : maxBits(bits) {
            root = new TrieNode();
        }
        
        void insert(int num) {
            TrieNode* curr = root;
            for (int i = maxBits; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!curr->children[bit]) {
                    curr->children[bit] = new TrieNode();
                }
                curr = curr->children[bit];
                curr->count++;
            }
        }
        
        void remove(int num) {
            TrieNode* curr = root;
            for (int i = maxBits; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!curr->children[bit]) return;
                curr = curr->children[bit];
                curr->count--;
            }
        }
        
        int findMaxXOR(int num) {
            TrieNode* curr = root;
            int result = 0;
            
            for (int i = maxBits; i >= 0; i--) {
                int bit = (num >> i) & 1;
                int oppositeBit = 1 - bit;
                
                if (curr->children[oppositeBit] && curr->children[oppositeBit]->count > 0) {
                    result |= (1 << i);
                    curr = curr->children[oppositeBit];
                } else if (curr->children[bit] && curr->children[bit]->count > 0) {
                    curr = curr->children[bit];
                } else {
                    return -1; // No valid XOR found
                }
            }
            
            return result;
        }
        
        int findMinXOR(int num) {
            TrieNode* curr = root;
            int result = 0;
            
            for (int i = maxBits; i >= 0; i--) {
                int bit = (num >> i) & 1;
                
                if (curr->children[bit] && curr->children[bit]->count > 0) {
                    curr = curr->children[bit];
                } else if (curr->children[1 - bit] && curr->children[1 - bit]->count > 0) {
                    result |= (1 << i);
                    curr = curr->children[1 - bit];
                } else {
                    return -1; // No valid XOR found
                }
            }
            
            return result;
        }
        
        bool contains(int num) {
            TrieNode* curr = root;
            for (int i = maxBits; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!curr->children[bit] || curr->children[bit]->count == 0) {
                    return false;
                }
                curr = curr->children[bit];
            }
            return true;
        }
    };
    
    // Compressed Trie (Radix Tree)
    class CompressedTrie {
    public:
        struct TrieNode {
            map<string, TrieNode*> children;
            bool isEndOfWord;
            string edgeLabel;
            
            TrieNode(string label = "") : isEndOfWord(false), edgeLabel(label) {}
        };
        
        TrieNode* root;
        
        CompressedTrie() {
            root = new TrieNode();
        }
        
        void insert(string word) {
            TrieNode* curr = root;
            int i = 0;
            
            while (i < word.length()) {
                bool found = false;
                
                for (auto& [label, child] : curr->children) {
                    int commonLen = getCommonPrefixLength(word.substr(i), label);
                    
                    if (commonLen > 0) {
                        found = true;
                        
                        if (commonLen == label.length()) {
                            // Full match with edge label
                            curr = child;
                            i += commonLen;
                        } else {
                            // Partial match - need to split edge
                            TrieNode* newNode = new TrieNode(label.substr(commonLen));
                            newNode->children = child->children;
                            newNode->isEndOfWord = child->isEndOfWord;
                            
                            child->children.clear();
                            child->children[label.substr(commonLen)] = newNode;
                            child->edgeLabel = label.substr(0, commonLen);
                            child->isEndOfWord = false;
                            
                            curr->children.erase(label);
                            curr->children[label.substr(0, commonLen)] = child;
                            
                            curr = child;
                            i += commonLen;
                        }
                        break;
                    }
                }
                
                if (!found) {
                    // No matching edge found - create new edge
                    TrieNode* newNode = new TrieNode(word.substr(i));
                    newNode->isEndOfWord = true;
                    curr->children[word.substr(i)] = newNode;
                    return;
                }
            }
            
            curr->isEndOfWord = true;
        }
        
        bool search(string word) {
            TrieNode* curr = root;
            int i = 0;
            
            while (i < word.length()) {
                bool found = false;
                
                for (auto& [label, child] : curr->children) {
                    if (word.substr(i).substr(0, label.length()) == label) {
                        curr = child;
                        i += label.length();
                        found = true;
                        break;
                    }
                }
                
                if (!found) return false;
            }
            
            return curr->isEndOfWord;
        }
        
    private:
        int getCommonPrefixLength(string s1, string s2) {
            int len = 0;
            int minLen = min(s1.length(), s2.length());
            
            while (len < minLen && s1[len] == s2[len]) {
                len++;
            }
            
            return len;
        }
    };
    
    // Persistent Trie
    class PersistentTrie {
    public:
        struct TrieNode {
            map<char, TrieNode*> children;
            bool isEndOfWord;
            int version;
            
            TrieNode(int v = 0) : isEndOfWord(false), version(v) {}
        };
        
        vector<TrieNode*> roots;
        int currentVersion;
        
        PersistentTrie() : currentVersion(0) {
            roots.push_back(new TrieNode(0));
        }
        
        void insert(string word) {
            TrieNode* newRoot = copyNode(roots[currentVersion]);
            TrieNode* curr = newRoot;
            
            for (char c : word) {
                if (curr->children.find(c) == curr->children.end()) {
                    curr->children[c] = new TrieNode(currentVersion + 1);
                } else {
                    curr->children[c] = copyNode(curr->children[c]);
                }
                curr = curr->children[c];
            }
            
            curr->isEndOfWord = true;
            currentVersion++;
            roots.push_back(newRoot);
        }
        
        bool search(string word, int version = -1) {
            if (version == -1) version = currentVersion;
            if (version >= roots.size()) return false;
            
            TrieNode* curr = roots[version];
            for (char c : word) {
                if (curr->children.find(c) == curr->children.end()) {
                    return false;
                }
                curr = curr->children[c];
            }
            return curr->isEndOfWord;
        }
        
    private:
        TrieNode* copyNode(TrieNode* node) {
            TrieNode* newNode = new TrieNode(currentVersion + 1);
            newNode->children = node->children;
            newNode->isEndOfWord = node->isEndOfWord;
            return newNode;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace TrieStructures;
    
    // Basic Trie
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    
    cout << "Search 'app': " << trie.search("app") << endl;
    cout << "Starts with 'app': " << trie.startsWith("app") << endl;
    cout << "Words with prefix 'app': " << trie.countWordsWithPrefix("app") << endl;
    
    auto words = trie.getWordsWithPrefix("app");
    cout << "All words with prefix 'app': ";
    for (string word : words) {
        cout << word << " ";
    }
    cout << endl;
    
    // Binary Trie
    BinaryTrie binaryTrie;
    binaryTrie.insert(5);  // 101
    binaryTrie.insert(3);  // 011
    binaryTrie.insert(7);  // 111
    
    cout << "Max XOR with 2: " << binaryTrie.findMaxXOR(2) << endl;
    cout << "Min XOR with 2: " << binaryTrie.findMinXOR(2) << endl;
    
    return 0;
}
*/