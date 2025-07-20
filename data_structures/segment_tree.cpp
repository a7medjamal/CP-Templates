#include <bits/stdc++.h>
using namespace std;

// Segment Tree for range sum queries and point updates
class SegmentTree {
public:
    vector<long long> tree;
    int n;
    
    SegmentTree(vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }
    
    void build(vector<int>& arr, int node, int start, int end) {
        if(start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }
    
    void update(int node, int start, int end, int idx, int val) {
        if(start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if(idx <= mid) {
                update(2 * node, start, mid, idx, val);
            } else {
                update(2 * node + 1, mid + 1, end, idx, val);
            }
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }
    
    long long query(int node, int start, int end, int l, int r) {
        if(r < start || end < l) {
            return 0; // Outside range
        }
        if(l <= start && end <= r) {
            return tree[node]; // Completely inside range
        }
        int mid = (start + end) / 2;
        return query(2 * node, start, mid, l, r) + 
               query(2 * node + 1, mid + 1, end, l, r);
    }
    
    // Public interface
    void update(int idx, int val) {
        update(1, 0, n - 1, idx, val);
    }
    
    long long query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }
};

// Lazy Propagation Segment Tree for range updates
class LazySegmentTree {
public:
    vector<long long> tree, lazy;
    int n;
    
    LazySegmentTree(vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        lazy.resize(4 * n, 0);
        build(arr, 1, 0, n - 1);
    }
    
    void build(vector<int>& arr, int node, int start, int end) {
        if(start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }
    
    void push(int node, int start, int end) {
        if(lazy[node] != 0) {
            tree[node] += lazy[node] * (end - start + 1);
            if(start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }
    
    void updateRange(int node, int start, int end, int l, int r, int val) {
        push(node, start, end);
        if(start > r || end < l) return;
        
        if(start >= l && end <= r) {
            lazy[node] += val;
            push(node, start, end);
            return;
        }
        
        int mid = (start + end) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        
        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    long long queryRange(int node, int start, int end, int l, int r) {
        if(start > r || end < l) return 0;
        push(node, start, end);
        
        if(start >= l && end <= r) {
            return tree[node];
        }
        
        int mid = (start + end) / 2;
        return queryRange(2 * node, start, mid, l, r) + 
               queryRange(2 * node + 1, mid + 1, end, l, r);
    }
    
    // Public interface
    void updateRange(int l, int r, int val) {
        updateRange(1, 0, n - 1, l, r, val);
    }
    
    long long queryRange(int l, int r) {
        return queryRange(1, 0, n - 1, l, r);
    }
};

// Example usage:
/*
int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    SegmentTree st(arr);
    
    cout << st.query(1, 3) << endl; // Sum from index 1 to 3: 3+5+7 = 15
    st.update(1, 10);               // Update index 1 to 10
    cout << st.query(1, 3) << endl; // Sum from index 1 to 3: 10+5+7 = 22
    
    return 0;
}
*/