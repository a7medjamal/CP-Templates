#include <bits/stdc++.h>
using namespace std;

// Balanced Tree Data Structures
namespace BalancedTrees {
    
    // Treap (Randomized Binary Search Tree)
    class Treap {
    private:
        struct Node {
            int key, priority, size;
            Node* left;
            Node* right;
            
            Node(int k) : key(k), priority(rand()), size(1), left(nullptr), right(nullptr) {}
        };
        
        Node* root;
        
        int getSize(Node* node) {
            return node ? node->size : 0;
        }
        
        void updateSize(Node* node) {
            if (node) {
                node->size = 1 + getSize(node->left) + getSize(node->right);
            }
        }
        
        void split(Node* node, int key, Node*& left, Node*& right) {
            if (!node) {
                left = right = nullptr;
                return;
            }
            
            if (node->key <= key) {
                split(node->right, key, node->right, right);
                left = node;
            } else {
                split(node->left, key, left, node->left);
                right = node;
            }
            updateSize(node);
        }
        
        Node* merge(Node* left, Node* right) {
            if (!left || !right) {
                return left ? left : right;
            }
            
            if (left->priority > right->priority) {
                left->right = merge(left->right, right);
                updateSize(left);
                return left;
            } else {
                right->left = merge(left, right->left);
                updateSize(right);
                return right;
            }
        }
        
        Node* insert(Node* node, int key) {
            if (!node) {
                return new Node(key);
            }
            
            if (key < node->key) {
                node->left = insert(node->left, key);
            } else if (key > node->key) {
                node->right = insert(node->right, key);
            } else {
                return node; // Key already exists
            }
            
            updateSize(node);
            
            // Maintain heap property
            if (node->left && node->left->priority > node->priority) {
                return rotateRight(node);
            }
            if (node->right && node->right->priority > node->priority) {
                return rotateLeft(node);
            }
            
            return node;
        }
        
        Node* rotateLeft(Node* node) {
            Node* newRoot = node->right;
            node->right = newRoot->left;
            newRoot->left = node;
            updateSize(node);
            updateSize(newRoot);
            return newRoot;
        }
        
        Node* rotateRight(Node* node) {
            Node* newRoot = node->left;
            node->left = newRoot->right;
            newRoot->right = node;
            updateSize(node);
            updateSize(newRoot);
            return newRoot;
        }
        
        Node* remove(Node* node, int key) {
            if (!node) return nullptr;
            
            if (key < node->key) {
                node->left = remove(node->left, key);
            } else if (key > node->key) {
                node->right = remove(node->right, key);
            } else {
                Node* result = merge(node->left, node->right);
                delete node;
                return result;
            }
            
            updateSize(node);
            return node;
        }
        
        bool find(Node* node, int key) {
            if (!node) return false;
            if (key == node->key) return true;
            return key < node->key ? find(node->left, key) : find(node->right, key);
        }
        
        int kthElement(Node* node, int k) {
            if (!node) return -1;
            
            int leftSize = getSize(node->left);
            if (k == leftSize + 1) {
                return node->key;
            } else if (k <= leftSize) {
                return kthElement(node->left, k);
            } else {
                return kthElement(node->right, k - leftSize - 1);
            }
        }
        
        void inorder(Node* node, vector<int>& result) {
            if (!node) return;
            inorder(node->left, result);
            result.push_back(node->key);
            inorder(node->right, result);
        }
        
    public:
        Treap() : root(nullptr) {
            srand(time(nullptr));
        }
        
        void insert(int key) {
            root = insert(root, key);
        }
        
        void remove(int key) {
            root = remove(root, key);
        }
        
        bool find(int key) {
            return find(root, key);
        }
        
        int size() {
            return getSize(root);
        }
        
        int kthElement(int k) {
            return kthElement(root, k);
        }
        
        vector<int> inorderTraversal() {
            vector<int> result;
            inorder(root, result);
            return result;
        }
    };
    
    // Splay Tree
    class SplayTree {
    private:
        struct Node {
            int key;
            Node* left;
            Node* right;
            Node* parent;
            
            Node(int k) : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
        };
        
        Node* root;
        
        void setParent(Node* child, Node* parent) {
            if (child) child->parent = parent;
        }
        
        void keepParent(Node* node) {
            setParent(node->left, node);
            setParent(node->right, node);
        }
        
        void rotate(Node* child, Node* parent) {
            Node* gparent = parent->parent;
            if (gparent) {
                if (gparent->left == parent) gparent->left = child;
                else gparent->right = child;
            }
            
            if (parent->left == child) {
                parent->left = child->right;
                child->right = parent;
            } else {
                parent->right = child->left;
                child->left = parent;
            }
            
            keepParent(child);
            keepParent(parent);
            child->parent = gparent;
        }
        
        void splay(Node* node) {
            if (!node) return;
            
            while (node->parent) {
                Node* parent = node->parent;
                Node* gparent = parent->parent;
                
                if (!gparent) {
                    rotate(node, parent);
                } else if ((gparent->left == parent) == (parent->left == node)) {
                    rotate(parent, gparent);
                    rotate(node, parent);
                } else {
                    rotate(node, parent);
                    rotate(node, gparent);
                }
            }
            root = node;
        }
        
        Node* find(Node* node, int key) {
            if (!node) return nullptr;
            if (key == node->key) return node;
            return key < node->key ? find(node->left, key) : find(node->right, key);
        }
        
        Node* insert(Node* node, Node* parent, int key) {
            if (!node) {
                Node* newNode = new Node(key);
                newNode->parent = parent;
                return newNode;
            }
            
            if (key < node->key) {
                node->left = insert(node->left, node, key);
            } else if (key > node->key) {
                node->right = insert(node->right, node, key);
            }
            
            return node;
        }
        
        void inorder(Node* node, vector<int>& result) {
            if (!node) return;
            inorder(node->left, result);
            result.push_back(node->key);
            inorder(node->right, result);
        }
        
    public:
        SplayTree() : root(nullptr) {}
        
        void insert(int key) {
            root = insert(root, nullptr, key);
            Node* node = find(root, key);
            if (node) splay(node);
        }
        
        bool find(int key) {
            Node* node = find(root, key);
            if (node) {
                splay(node);
                return true;
            }
            return false;
        }
        
        vector<int> inorderTraversal() {
            vector<int> result;
            inorder(root, result);
            return result;
        }
    };
    
    // AVL Tree
    class AVLTree {
    private:
        struct Node {
            int key, height;
            Node* left;
            Node* right;
            
            Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
        };
        
        Node* root;
        
        int getHeight(Node* node) {
            return node ? node->height : 0;
        }
        
        int getBalance(Node* node) {
            return node ? getHeight(node->left) - getHeight(node->right) : 0;
        }
        
        void updateHeight(Node* node) {
            if (node) {
                node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            }
        }
        
        Node* rotateRight(Node* y) {
            Node* x = y->left;
            Node* T2 = x->right;
            
            x->right = y;
            y->left = T2;
            
            updateHeight(y);
            updateHeight(x);
            
            return x;
        }
        
        Node* rotateLeft(Node* x) {
            Node* y = x->right;
            Node* T2 = y->left;
            
            y->left = x;
            x->right = T2;
            
            updateHeight(x);
            updateHeight(y);
            
            return y;
        }
        
        Node* insert(Node* node, int key) {
            if (!node) {
                return new Node(key);
            }
            
            if (key < node->key) {
                node->left = insert(node->left, key);
            } else if (key > node->key) {
                node->right = insert(node->right, key);
            } else {
                return node; // Duplicate keys not allowed
            }
            
            updateHeight(node);
            
            int balance = getBalance(node);
            
            // Left Left Case
            if (balance > 1 && key < node->left->key) {
                return rotateRight(node);
            }
            
            // Right Right Case
            if (balance < -1 && key > node->right->key) {
                return rotateLeft(node);
            }
            
            // Left Right Case
            if (balance > 1 && key > node->left->key) {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            
            // Right Left Case
            if (balance < -1 && key < node->right->key) {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            
            return node;
        }
        
        Node* findMin(Node* node) {
            while (node && node->left) {
                node = node->left;
            }
            return node;
        }
        
        Node* remove(Node* node, int key) {
            if (!node) return node;
            
            if (key < node->key) {
                node->left = remove(node->left, key);
            } else if (key > node->key) {
                node->right = remove(node->right, key);
            } else {
                if (!node->left || !node->right) {
                    Node* temp = node->left ? node->left : node->right;
                    if (!temp) {
                        temp = node;
                        node = nullptr;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    Node* temp = findMin(node->right);
                    node->key = temp->key;
                    node->right = remove(node->right, temp->key);
                }
            }
            
            if (!node) return node;
            
            updateHeight(node);
            
            int balance = getBalance(node);
            
            // Left Left Case
            if (balance > 1 && getBalance(node->left) >= 0) {
                return rotateRight(node);
            }
            
            // Left Right Case
            if (balance > 1 && getBalance(node->left) < 0) {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            
            // Right Right Case
            if (balance < -1 && getBalance(node->right) <= 0) {
                return rotateLeft(node);
            }
            
            // Right Left Case
            if (balance < -1 && getBalance(node->right) > 0) {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            
            return node;
        }
        
        bool find(Node* node, int key) {
            if (!node) return false;
            if (key == node->key) return true;
            return key < node->key ? find(node->left, key) : find(node->right, key);
        }
        
        void inorder(Node* node, vector<int>& result) {
            if (!node) return;
            inorder(node->left, result);
            result.push_back(node->key);
            inorder(node->right, result);
        }
        
    public:
        AVLTree() : root(nullptr) {}
        
        void insert(int key) {
            root = insert(root, key);
        }
        
        void remove(int key) {
            root = remove(root, key);
        }
        
        bool find(int key) {
            return find(root, key);
        }
        
        vector<int> inorderTraversal() {
            vector<int> result;
            inorder(root, result);
            return result;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace BalancedTrees;
    
    // Treap example
    Treap treap;
    treap.insert(10);
    treap.insert(5);
    treap.insert(15);
    treap.insert(3);
    treap.insert(7);
    
    cout << "Treap size: " << treap.size() << endl;
    cout << "3rd element: " << treap.kthElement(3) << endl;
    
    // AVL Tree example
    AVLTree avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(30);
    avl.insert(40);
    avl.insert(50);
    
    auto result = avl.inorderTraversal();
    cout << "AVL inorder: ";
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;
    
    return 0;
}
*/