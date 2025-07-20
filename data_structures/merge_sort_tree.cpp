#include <bits/stdc++.h>
using namespace std;

// Merge Sort Tree for range queries on sorted subarrays
namespace MergeSortTree {
    
    class MergeSortTree {
    private:
        vector<vector<int>> tree;
        int n;
        
        void build(vector<int>& arr, int node, int start, int end) {
            if (start == end) {
                tree[node].push_back(arr[start]);
            } else {
                int mid = (start + end) / 2;
                build(arr, 2 * node, start, mid);
                build(arr, 2 * node + 1, mid + 1, end);
                
                // Merge the two sorted arrays
                merge(tree[2 * node].begin(), tree[2 * node].end(),
                      tree[2 * node + 1].begin(), tree[2 * node + 1].end(),
                      back_inserter(tree[node]));
            }
        }
        
        int query(int node, int start, int end, int l, int r, int x) {
            if (r < start || end < l) {
                return 0;
            }
            if (l <= start && end <= r) {
                // Count elements <= x in this range
                return upper_bound(tree[node].begin(), tree[node].end(), x) - tree[node].begin();
            }
            
            int mid = (start + end) / 2;
            return query(2 * node, start, mid, l, r, x) + 
                   query(2 * node + 1, mid + 1, end, l, r, x);
        }
        
        int queryRange(int node, int start, int end, int l, int r, int low, int high) {
            if (r < start || end < l) {
                return 0;
            }
            if (l <= start && end <= r) {
                // Count elements in range [low, high]
                return upper_bound(tree[node].begin(), tree[node].end(), high) - 
                       lower_bound(tree[node].begin(), tree[node].end(), low);
            }
            
            int mid = (start + end) / 2;
            return queryRange(2 * node, start, mid, l, r, low, high) + 
                   queryRange(2 * node + 1, mid + 1, end, l, r, low, high);
        }
        
        int kthSmallest(int node, int start, int end, int l, int r, int k) {
            if (start == end) {
                return tree[node][0];
            }
            
            int mid = (start + end) / 2;
            int leftCount = 0;
            
            // Count how many elements from [l, r] are in left subtree
            if (l <= mid) {
                leftCount = min(mid, r) - l + 1;
            }
            
            if (k <= leftCount) {
                return kthSmallest(2 * node, start, mid, l, min(mid, r), k);
            } else {
                return kthSmallest(2 * node + 1, mid + 1, end, max(mid + 1, l), r, k - leftCount);
            }
        }
        
    public:
        MergeSortTree(vector<int>& arr) {
            n = arr.size();
            tree.resize(4 * n);
            build(arr, 1, 0, n - 1);
        }
        
        // Count elements <= x in range [l, r]
        int countLessEqual(int l, int r, int x) {
            return query(1, 0, n - 1, l, r, x);
        }
        
        // Count elements in range [low, high] within array range [l, r]
        int countInRange(int l, int r, int low, int high) {
            return queryRange(1, 0, n - 1, l, r, low, high);
        }
        
        // Find kth smallest element in range [l, r] (1-indexed)
        int kthSmallest(int l, int r, int k) {
            return kthSmallest(1, 0, n - 1, l, r, k);
        }
        
        // Count elements > x in range [l, r]
        int countGreater(int l, int r, int x) {
            return (r - l + 1) - countLessEqual(l, r, x);
        }
        
        // Count elements < x in range [l, r]
        int countLess(int l, int r, int x) {
            return countLessEqual(l, r, x - 1);
        }
    };
    
    // Persistent Merge Sort Tree (for updates)
    class PersistentMergeSortTree {
    private:
        struct Node {
            vector<int> values;
            Node* left;
            Node* right;
            
            Node() : left(nullptr), right(nullptr) {}
        };
        
        Node* root;
        int n;
        
        Node* build(vector<int>& arr, int start, int end) {
            Node* node = new Node();
            
            if (start == end) {
                node->values.push_back(arr[start]);
            } else {
                int mid = (start + end) / 2;
                node->left = build(arr, start, mid);
                node->right = build(arr, mid + 1, end);
                
                // Merge the two sorted arrays
                merge(node->left->values.begin(), node->left->values.end(),
                      node->right->values.begin(), node->right->values.end(),
                      back_inserter(node->values));
            }
            
            return node;
        }
        
        int query(Node* node, int start, int end, int l, int r, int x) {
            if (!node || r < start || end < l) {
                return 0;
            }
            if (l <= start && end <= r) {
                return upper_bound(node->values.begin(), node->values.end(), x) - node->values.begin();
            }
            
            int mid = (start + end) / 2;
            return query(node->left, start, mid, l, r, x) + 
                   query(node->right, mid + 1, end, l, r, x);
        }
        
    public:
        PersistentMergeSortTree(vector<int>& arr) {
            n = arr.size();
            root = build(arr, 0, n - 1);
        }
        
        int countLessEqual(int l, int r, int x) {
            return query(root, 0, n - 1, l, r, x);
        }
    };
    
    // Wavelet Tree (alternative implementation for similar queries)
    class WaveletTree {
    private:
        struct Node {
            int low, high;
            vector<int> prefix;
            Node* left;
            Node* right;
            
            Node(int l, int h) : low(l), high(h), left(nullptr), right(nullptr) {}
        };
        
        Node* root;
        vector<int> values;
        
        Node* build(vector<int>& arr, int start, int end, int low, int high) {
            if (start > end) return nullptr;
            
            Node* node = new Node(low, high);
            node->prefix.resize(end - start + 2, 0);
            
            if (low == high) {
                return node;
            }
            
            int mid = (low + high) / 2;
            vector<int> left_arr, right_arr;
            
            for (int i = start; i <= end; i++) {
                if (arr[i] <= mid) {
                    left_arr.push_back(arr[i]);
                    node->prefix[i - start + 1] = node->prefix[i - start] + 1;
                } else {
                    right_arr.push_back(arr[i]);
                    node->prefix[i - start + 1] = node->prefix[i - start];
                }
            }
            
            node->left = build(left_arr, 0, left_arr.size() - 1, low, mid);
            node->right = build(right_arr, 0, right_arr.size() - 1, mid + 1, high);
            
            return node;
        }
        
        int kthSmallest(Node* node, int l, int r, int k) {
            if (!node || node->low == node->high) {
                return node->low;
            }
            
            int leftCount = node->prefix[r + 1] - node->prefix[l];
            
            if (k <= leftCount) {
                int newL = node->prefix[l];
                int newR = node->prefix[r + 1] - 1;
                return kthSmallest(node->left, newL, newR, k);
            } else {
                int newL = l - node->prefix[l];
                int newR = r - node->prefix[r + 1];
                return kthSmallest(node->right, newL, newR, k - leftCount);
            }
        }
        
    public:
        WaveletTree(vector<int>& arr) {
            values = arr;
            sort(values.begin(), values.end());
            values.erase(unique(values.begin(), values.end()), values.end());
            
            // Compress coordinates
            for (int& x : arr) {
                x = lower_bound(values.begin(), values.end(), x) - values.begin();
            }
            
            root = build(arr, 0, arr.size() - 1, 0, values.size() - 1);
        }
        
        int kthSmallest(int l, int r, int k) {
            int compressed = kthSmallest(root, l, r, k);
            return values[compressed];
        }
    };
}

// Example usage:
/*
int main() {
    using namespace MergeSortTree;
    
    vector<int> arr = {2, 1, 4, 3, 5, 7, 6, 8};
    MergeSortTree mst(arr);
    
    // Count elements <= 4 in range [1, 5]
    cout << "Elements <= 4 in range [1, 5]: " << mst.countLessEqual(1, 5, 4) << endl;
    
    // Count elements in range [3, 6] within array range [0, 6]
    cout << "Elements in range [3, 6]: " << mst.countInRange(0, 6, 3, 6) << endl;
    
    // Find 3rd smallest element in range [2, 6]
    cout << "3rd smallest in range [2, 6]: " << mst.kthSmallest(2, 6, 3) << endl;
    
    return 0;
}
*/