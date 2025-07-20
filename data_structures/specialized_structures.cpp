#include <bits/stdc++.h>
using namespace std;

// Specialized Data Structures
namespace SpecializedStructures {
    
    // Skip List
    class SkipList {
    private:
        static const int MAX_LEVEL = 16;
        
        struct Node {
            int value;
            vector<Node*> forward;
            
            Node(int val, int level) : value(val), forward(level + 1, nullptr) {}
        };
        
        Node* header;
        int level;
        
        int randomLevel() {
            int lvl = 0;
            while (rand() % 2 && lvl < MAX_LEVEL) {
                lvl++;
            }
            return lvl;
        }
        
    public:
        SkipList() {
            header = new Node(-1, MAX_LEVEL);
            level = 0;
            srand(time(nullptr));
        }
        
        void insert(int value) {
            vector<Node*> update(MAX_LEVEL + 1);
            Node* current = header;
            
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] && current->forward[i]->value < value) {
                    current = current->forward[i];
                }
                update[i] = current;
            }
            
            current = current->forward[0];
            
            if (!current || current->value != value) {
                int newLevel = randomLevel();
                
                if (newLevel > level) {
                    for (int i = level + 1; i <= newLevel; i++) {
                        update[i] = header;
                    }
                    level = newLevel;
                }
                
                Node* newNode = new Node(value, newLevel);
                
                for (int i = 0; i <= newLevel; i++) {
                    newNode->forward[i] = update[i]->forward[i];
                    update[i]->forward[i] = newNode;
                }
            }
        }
        
        bool search(int value) {
            Node* current = header;
            
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] && current->forward[i]->value < value) {
                    current = current->forward[i];
                }
            }
            
            current = current->forward[0];
            return current && current->value == value;
        }
        
        void remove(int value) {
            vector<Node*> update(MAX_LEVEL + 1);
            Node* current = header;
            
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] && current->forward[i]->value < value) {
                    current = current->forward[i];
                }
                update[i] = current;
            }
            
            current = current->forward[0];
            
            if (current && current->value == value) {
                for (int i = 0; i <= level; i++) {
                    if (update[i]->forward[i] != current) break;
                    update[i]->forward[i] = current->forward[i];
                }
                
                delete current;
                
                while (level > 0 && !header->forward[level]) {
                    level--;
                }
            }
        }
        
        void display() {
            for (int i = level; i >= 0; i--) {
                Node* node = header->forward[i];
                cout << "Level " << i << ": ";
                while (node) {
                    cout << node->value << " ";
                    node = node->forward[i];
                }
                cout << endl;
            }
        }
    };
    
    // B-Tree (simplified version)
    template<int T>
    class BTree {
    private:
        struct Node {
            vector<int> keys;
            vector<Node*> children;
            bool isLeaf;
            
            Node(bool leaf) : isLeaf(leaf) {
                keys.reserve(2 * T - 1);
                children.reserve(2 * T);
            }
        };
        
        Node* root;
        
        void splitChild(Node* parent, int index) {
            Node* fullChild = parent->children[index];
            Node* newChild = new Node(fullChild->isLeaf);
            
            newChild->keys.assign(fullChild->keys.begin() + T, fullChild->keys.end());
            fullChild->keys.resize(T - 1);
            
            if (!fullChild->isLeaf) {
                newChild->children.assign(fullChild->children.begin() + T, fullChild->children.end());
                fullChild->children.resize(T);
            }
            
            parent->children.insert(parent->children.begin() + index + 1, newChild);
            parent->keys.insert(parent->keys.begin() + index, fullChild->keys[T - 1]);
        }
        
        void insertNonFull(Node* node, int key) {
            int i = node->keys.size() - 1;
            
            if (node->isLeaf) {
                node->keys.push_back(0);
                while (i >= 0 && node->keys[i] > key) {
                    node->keys[i + 1] = node->keys[i];
                    i--;
                }
                node->keys[i + 1] = key;
            } else {
                while (i >= 0 && node->keys[i] > key) {
                    i--;
                }
                i++;
                
                if (node->children[i]->keys.size() == 2 * T - 1) {
                    splitChild(node, i);
                    if (node->keys[i] < key) {
                        i++;
                    }
                }
                insertNonFull(node->children[i], key);
            }
        }
        
        bool search(Node* node, int key) {
            if (!node) return false;
            
            int i = 0;
            while (i < node->keys.size() && key > node->keys[i]) {
                i++;
            }
            
            if (i < node->keys.size() && key == node->keys[i]) {
                return true;
            }
            
            if (node->isLeaf) {
                return false;
            }
            
            return search(node->children[i], key);
        }
        
        void inorder(Node* node, vector<int>& result) {
            if (!node) return;
            
            int i;
            for (i = 0; i < node->keys.size(); i++) {
                if (!node->isLeaf) {
                    inorder(node->children[i], result);
                }
                result.push_back(node->keys[i]);
            }
            
            if (!node->isLeaf) {
                inorder(node->children[i], result);
            }
        }
        
    public:
        BTree() {
            root = new Node(true);
        }
        
        void insert(int key) {
            if (root->keys.size() == 2 * T - 1) {
                Node* newRoot = new Node(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0);
                root = newRoot;
            }
            insertNonFull(root, key);
        }
        
        bool search(int key) {
            return search(root, key);
        }
        
        vector<int> inorderTraversal() {
            vector<int> result;
            inorder(root, result);
            return result;
        }
    };
    
    // Cartesian Tree
    class CartesianTree {
    private:
        struct Node {
            int value, index;
            Node* left;
            Node* right;
            
            Node(int val, int idx) : value(val), index(idx), left(nullptr), right(nullptr) {}
        };
        
        Node* root;
        
        Node* buildTree(vector<int>& arr, int start, int end) {
            if (start > end) return nullptr;
            
            int minIdx = start;
            for (int i = start + 1; i <= end; i++) {
                if (arr[i] < arr[minIdx]) {
                    minIdx = i;
                }
            }
            
            Node* node = new Node(arr[minIdx], minIdx);
            node->left = buildTree(arr, start, minIdx - 1);
            node->right = buildTree(arr, minIdx + 1, end);
            
            return node;
        }
        
        void inorder(Node* node, vector<pair<int, int>>& result) {
            if (!node) return;
            inorder(node->left, result);
            result.push_back({node->value, node->index});
            inorder(node->right, result);
        }
        
        void preorder(Node* node, vector<pair<int, int>>& result) {
            if (!node) return;
            result.push_back({node->value, node->index});
            preorder(node->left, result);
            preorder(node->right, result);
        }
        
    public:
        CartesianTree(vector<int>& arr) {
            if (!arr.empty()) {
                root = buildTree(arr, 0, arr.size() - 1);
            } else {
                root = nullptr;
            }
        }
        
        vector<pair<int, int>> inorderTraversal() {
            vector<pair<int, int>> result;
            inorder(root, result);
            return result;
        }
        
        vector<pair<int, int>> preorderTraversal() {
            vector<pair<int, int>> result;
            preorder(root, result);
            return result;
        }
    };
    
    // Rope Data Structure (for string operations)
    class Rope {
    private:
        struct Node {
            string str;
            int weight;
            Node* left;
            Node* right;
            
            Node(const string& s) : str(s), weight(s.length()), left(nullptr), right(nullptr) {}
            Node() : weight(0), left(nullptr), right(nullptr) {}
        };
        
        Node* root;
        
        int getWeight(Node* node) {
            return node ? node->weight : 0;
        }
        
        void updateWeight(Node* node) {
            if (node) {
                node->weight = getWeight(node->left) + (node->str.empty() ? 0 : node->str.length());
            }
        }
        
        char charAt(Node* node, int index) {
            if (!node) return '\0';
            
            int leftWeight = getWeight(node->left);
            
            if (index < leftWeight) {
                return charAt(node->left, index);
            } else if (index < leftWeight + node->str.length()) {
                return node->str[index - leftWeight];
            } else {
                return charAt(node->right, index - leftWeight - node->str.length());
            }
        }
        
        Node* concatenate(Node* left, Node* right) {
            if (!left) return right;
            if (!right) return left;
            
            Node* newNode = new Node();
            newNode->left = left;
            newNode->right = right;
            updateWeight(newNode);
            
            return newNode;
        }
        
        pair<Node*, Node*> split(Node* node, int index) {
            if (!node) return {nullptr, nullptr};
            
            int leftWeight = getWeight(node->left);
            
            if (index <= leftWeight) {
                auto [leftPart, rightPart] = split(node->left, index);
                Node* newRight = new Node();
                newRight->str = node->str;
                newRight->left = rightPart;
                newRight->right = node->right;
                updateWeight(newRight);
                return {leftPart, newRight};
            } else if (index <= leftWeight + node->str.length()) {
                int splitPos = index - leftWeight;
                Node* leftNode = new Node(node->str.substr(0, splitPos));
                Node* rightNode = new Node(node->str.substr(splitPos));
                
                leftNode->left = node->left;
                rightNode->right = node->right;
                
                updateWeight(leftNode);
                updateWeight(rightNode);
                
                return {leftNode, rightNode};
            } else {
                auto [leftPart, rightPart] = split(node->right, index - leftWeight - node->str.length());
                Node* newLeft = new Node();
                newLeft->str = node->str;
                newLeft->left = node->left;
                newLeft->right = leftPart;
                updateWeight(newLeft);
                return {newLeft, rightPart};
            }
        }
        
        string toString(Node* node) {
            if (!node) return "";
            return toString(node->left) + node->str + toString(node->right);
        }
        
    public:
        Rope(const string& str = "") {
            root = str.empty() ? nullptr : new Node(str);
        }
        
        char charAt(int index) {
            return charAt(root, index);
        }
        
        void insert(int index, const string& str) {
            auto [left, right] = split(root, index);
            Node* newNode = new Node(str);
            root = concatenate(concatenate(left, newNode), right);
        }
        
        void remove(int start, int length) {
            auto [left, temp] = split(root, start);
            auto [middle, right] = split(temp, length);
            root = concatenate(left, right);
        }
        
        string substring(int start, int length) {
            auto [left, temp] = split(root, start);
            auto [middle, right] = split(temp, length);
            string result = toString(middle);
            root = concatenate(concatenate(left, middle), right);
            return result;
        }
        
        string toString() {
            return toString(root);
        }
        
        int length() {
            return getWeight(root) + (root && !root->str.empty() ? root->str.length() : 0);
        }
    };
    
    // Suffix Tree (simplified)
    class SimpleSuffixTree {
    private:
        struct Node {
            map<char, Node*> children;
            bool isEndOfString;
            
            Node() : isEndOfString(false) {}
        };
        
        Node* root;
        
        void insert(const string& suffix) {
            Node* current = root;
            for (char c : suffix) {
                if (current->children.find(c) == current->children.end()) {
                    current->children[c] = new Node();
                }
                current = current->children[c];
            }
            current->isEndOfString = true;
        }
        
        bool search(Node* node, const string& pattern, int index) {
            if (index == pattern.length()) {
                return true;
            }
            
            char c = pattern[index];
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            
            return search(node->children[c], pattern, index + 1);
        }
        
    public:
        SimpleSuffixTree(const string& text) {
            root = new Node();
            for (int i = 0; i < text.length(); i++) {
                insert(text.substr(i));
            }
        }
        
        bool contains(const string& pattern) {
            return search(root, pattern, 0);
        }
    };
}

// Example usage:
/*
int main() {
    using namespace SpecializedStructures;
    
    // Skip List example
    SkipList skipList;
    skipList.insert(3);
    skipList.insert(6);
    skipList.insert(7);
    skipList.insert(9);
    skipList.insert(12);
    
    cout << "Search 6: " << (skipList.search(6) ? "Found" : "Not Found") << endl;
    skipList.display();
    
    // B-Tree example
    BTree<3> btree;
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    btree.insert(6);
    btree.insert(12);
    
    cout << "B-Tree search 6: " << (btree.search(6) ? "Found" : "Not Found") << endl;
    
    // Rope example
    Rope rope("Hello World");
    rope.insert(5, ", Beautiful");
    cout << "Rope: " << rope.toString() << endl;
    
    return 0;
}
*/