#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree (Binary Indexed Tree) for prefix sum queries
class FenwickTree {
public:
    vector<long long> tree;
    int n;
    
    FenwickTree(int size) {
        n = size;
        tree.assign(n + 1, 0);
    }
    
    FenwickTree(vector<int>& arr) {
        n = arr.size();
        tree.assign(n + 1, 0);
        for(int i = 0; i < n; i++) {
            update(i, arr[i]);
        }
    }
    
    void update(int idx, long long delta) {
        for(++idx; idx <= n; idx += idx & -idx) {
            tree[idx] += delta;
        }
    }
    
    long long query(int idx) {
        long long sum = 0;
        for(++idx; idx > 0; idx -= idx & -idx) {
            sum += tree[idx];
        }
        return sum;
    }
    
    long long rangeQuery(int l, int r) {
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
    
    // Find the largest index with prefix sum <= k
    int lowerBound(long long k) {
        int idx = 0;
        for(int bit = __builtin_clz(1) - __builtin_clz(n); bit >= 0; bit--) {
            int next = idx + (1 << bit);
            if(next <= n && tree[next] <= k) {
                k -= tree[next];
                idx = next;
            }
        }
        return idx;
    }
};

// 2D Fenwick Tree for 2D range sum queries
class FenwickTree2D {
public:
    vector<vector<long long>> tree;
    int n, m;
    
    FenwickTree2D(int rows, int cols) {
        n = rows;
        m = cols;
        tree.assign(n + 1, vector<long long>(m + 1, 0));
    }
    
    void update(int x, int y, long long delta) {
        for(int i = x + 1; i <= n; i += i & -i) {
            for(int j = y + 1; j <= m; j += j & -j) {
                tree[i][j] += delta;
            }
        }
    }
    
    long long query(int x, int y) {
        long long sum = 0;
        for(int i = x + 1; i > 0; i -= i & -i) {
            for(int j = y + 1; j > 0; j -= j & -j) {
                sum += tree[i][j];
            }
        }
        return sum;
    }
    
    long long rangeQuery(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
    }
};

// Example usage:
/*
int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    FenwickTree ft(arr);
    
    cout << ft.query(2) << endl;        // Prefix sum up to index 2: 1+3+5 = 9
    cout << ft.rangeQuery(1, 3) << endl; // Sum from index 1 to 3: 3+5+7 = 15
    
    ft.update(1, 7);                    // Add 7 to index 1 (3 becomes 10)
    cout << ft.rangeQuery(1, 3) << endl; // Sum from index 1 to 3: 10+5+7 = 22
    
    return 0;
}
*/