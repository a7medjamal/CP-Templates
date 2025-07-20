#include <bits/stdc++.h>
using namespace std;

// Link-Cut Tree for Dynamic Tree Connectivity
class LinkCutTree {
public:
    struct Node {
        Node *left, *right, *parent;
        long long value, sum;
        bool reversed;
        
        Node(long long val = 0) : left(nullptr), right(nullptr), parent(nullptr), 
                                 value(val), sum(val), reversed(false) {}
    };
    
    vector<Node*> nodes;
    
    LinkCutTree(int n) {
        nodes.resize(n);
        for (int i = 0; i < n; i++) {
            nodes[i] = new Node(0);
        }
    }
    
    void push(Node* x) {
        if (x->reversed) {
            swap(x->left, x->right);
            if (x->left) x->left->reversed ^= true;
            if (x->right) x->right->reversed ^= true;
            x->reversed = false;
        }
    }
    
    void update(Node* x) {
        x->sum = x->value;
        if (x->left) x->sum += x->left->sum;
        if (x->right) x->sum += x->right->sum;
    }
    
    bool isRoot(Node* x) {
        return !x->parent || (x->parent->left != x && x->parent->right != x);
    }
    
    void rotate(Node* x) {
        Node* p = x->parent;
        Node* g = p->parent;
        
        if (p->left == x) {
            p->left = x->right;
            if (x->right) x->right->parent = p;
            x->right = p;
        } else {
            p->right = x->left;
            if (x->left) x->left->parent = p;
            x->left = p;
        }
        
        p->parent = x;
        x->parent = g;
        
        if (g) {
            if (g->left == p) g->left = x;
            else if (g->right == p) g->right = x;
        }
        
        update(p);
        update(x);
    }
    
    void splay(Node* x) {
        while (!isRoot(x)) {
            Node* p = x->parent;
            Node* g = p->parent;
            
            if (!isRoot(p)) {
                push(g);
            }
            push(p);
            push(x);
            
            if (!isRoot(p)) {
                if ((g->left == p) == (p->left == x)) {
                    rotate(p);
                } else {
                    rotate(x);
                }
            }
            rotate(x);
        }
        push(x);
    }
    
    void access(Node* x) {
        Node* last = nullptr;
        while (x) {
            splay(x);
            x->right = last;
            update(x);
            last = x;
            x = x->parent;
        }
    }
    
    void makeRoot(Node* x) {
        access(x);
        splay(x);
        x->reversed ^= true;
    }
    
    void link(int u, int v) {
        makeRoot(nodes[u]);
        nodes[u]->parent = nodes[v];
    }
    
    void cut(int u, int v) {
        makeRoot(nodes[u]);
        access(nodes[v]);
        splay(nodes[v]);
        
        if (nodes[v]->left == nodes[u]) {
            nodes[v]->left = nullptr;
            nodes[u]->parent = nullptr;
            update(nodes[v]);
        }
    }
    
    long long pathSum(int u, int v) {
        makeRoot(nodes[u]);
        access(nodes[v]);
        splay(nodes[v]);
        return nodes[v]->sum;
    }
    
    bool connected(int u, int v) {
        makeRoot(nodes[u]);
        access(nodes[v]);
        splay(nodes[v]);
        return nodes[u]->parent != nullptr;
    }
};

// Example usage:
/*
int main() {
    LinkCutTree lct(5);
    
    // Link nodes
    lct.link(0, 1);
    lct.link(1, 2);
    lct.link(2, 3);
    
    // Check connectivity
    cout << "0 and 3 connected: " << lct.connected(0, 3) << endl;
    
    // Cut edge
    lct.cut(1, 2);
    cout << "0 and 3 connected after cut: " << lct.connected(0, 3) << endl;
    
    return 0;
}
*/