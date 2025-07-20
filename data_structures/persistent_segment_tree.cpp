#include <bits/stdc++.h>
using namespace std;

// Persistent Segment Tree for Historical Queries
class PersistentSegmentTree {
public:
    struct Node {
        Node* left;
        Node* right;
        long long sum;
        
        Node() : left(nullptr), right(nullptr), sum(0) {}
        Node(Node* l, Node* r, long long s) : left(l), right(r), sum(s) {}
    };
    
    vector<Node*> roots;
    int n;
    
    PersistentSegmentTree(vector<int>& arr) {
        n = arr.size();
        roots.push_back(build(arr, 0, n - 1));
    }
    
    Node* build(vector<int>& arr, int start, int end) {
        if (start == end) {
            return new Node(nullptr, nullptr, arr[start]);
        }
        
        int mid = (start + end) / 2;
        Node* left = build(arr, start, mid);
        Node* right = build(arr, mid + 1, end);
        
        return new Node(left, right, left->sum + right->sum);
    }
    
    Node* update(Node* node, int start, int end, int idx, int val) {
        if (start == end) {
            return new Node(nullptr, nullptr, val);
        }
        
        int mid = (start + end) / 2;
        if (idx <= mid) {
            Node* newLeft = update(node->left, start, mid, idx, val);
            return new Node(newLeft, node->right, newLeft->sum + node->right->sum);
        } else {
            Node* newRight = update(node->right, mid + 1, end, idx, val);
            return new Node(node->left, newRight, node->left->sum + newRight->sum);
        }
    }
    
    long long query(Node* node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return node->sum;
        
        int mid = (start + end) / 2;
        return query(node->left, start, mid, l, r) + 
               query(node->right, mid + 1, end, l, r);
    }
    
    void update(int version, int idx, int val) {
        Node* newRoot = update(roots[version], 0, n - 1, idx, val);
        roots.push_back(newRoot);
    }
    
    long long query(int version, int l, int r) {
        return query(roots[version], 0, n - 1, l, r);
    }
    
    int getVersionCount() {
        return roots.size();
    }
    
    // Kth smallest in range [l, r] using two versions
    int kthSmallest(int version1, int version2, int l, int r, int k) {
        return kthSmallestHelper(roots[version1], roots[version2], 0, n - 1, k);
    }
    
private:
    int kthSmallestHelper(Node* node1, Node* node2, int start, int end, int k) {
        if (start == end) {
            return start;
        }
        
        int mid = (start + end) / 2;
        long long leftCount = (node2->left ? node2->left->sum : 0) - 
                             (node1->left ? node1->left->sum : 0);
        
        if (k <= leftCount) {
            return kthSmallestHelper(node1->left, node2->left, start, mid, k);
        } else {
            return kthSmallestHelper(node1->right, node2->right, mid + 1, end, k - leftCount);
        }
    }
};

// Example usage:
/*
int main() {
    vector<int> arr = {1, 3, 2, 7, 9, 11};
    PersistentSegmentTree pst(arr);
    
    cout << "Initial sum [1, 3]: " << pst.query(0, 1, 3) << endl;
    
    // Update index 2 to value 5 (creates version 1)
    pst.update(0, 2, 5);
    
    cout << "After update sum [1, 3]: " << pst.query(1, 1, 3) << endl;
    cout << "Original sum [1, 3]: " << pst.query(0, 1, 3) << endl;
    
    return 0;
}
*/