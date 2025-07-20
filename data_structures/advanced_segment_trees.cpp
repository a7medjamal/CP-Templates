#include <bits/stdc++.h>
using namespace std;

// Advanced Segment Tree Variants
namespace AdvancedSegmentTrees {
    
    // Range Maximum Query Segment Tree
    class MaxSegmentTree {
    private:
        vector<long long> tree;
        int n;
        
        void build(vector<int>& arr, int node, int start, int end) {
            if (start == end) {
                tree[node] = arr[start];
            } else {
                int mid = (start + end) / 2;
                build(arr, 2 * node, start, mid);
                build(arr, 2 * node + 1, mid + 1, end);
                tree[node] = max(tree[2 * node], tree[2 * node + 1]);
            }
        }
        
        void update(int node, int start, int end, int idx, int val) {
            if (start == end) {
                tree[node] = val;
            } else {
                int mid = (start + end) / 2;
                if (idx <= mid) {
                    update(2 * node, start, mid, idx, val);
                } else {
                    update(2 * node + 1, mid + 1, end, idx, val);
                }
                tree[node] = max(tree[2 * node], tree[2 * node + 1]);
            }
        }
        
        long long query(int node, int start, int end, int l, int r) {
            if (r < start || end < l) {
                return LLONG_MIN;
            }
            if (l <= start && end <= r) {
                return tree[node];
            }
            int mid = (start + end) / 2;
            return max(query(2 * node, start, mid, l, r),
                      query(2 * node + 1, mid + 1, end, l, r));
        }
        
    public:
        MaxSegmentTree(vector<int>& arr) {
            n = arr.size();
            tree.resize(4 * n);
            build(arr, 1, 0, n - 1);
        }
        
        void update(int idx, int val) {
            update(1, 0, n - 1, idx, val);
        }
        
        long long query(int l, int r) {
            return query(1, 0, n - 1, l, r);
        }
    };
    
    // Range GCD Segment Tree
    class GCDSegmentTree {
    private:
        vector<long long> tree;
        int n;
        
        long long gcd(long long a, long long b) {
            return b ? gcd(b, a % b) : a;
        }
        
        void build(vector<int>& arr, int node, int start, int end) {
            if (start == end) {
                tree[node] = arr[start];
            } else {
                int mid = (start + end) / 2;
                build(arr, 2 * node, start, mid);
                build(arr, 2 * node + 1, mid + 1, end);
                tree[node] = gcd(tree[2 * node], tree[2 * node + 1]);
            }
        }
        
        void update(int node, int start, int end, int idx, int val) {
            if (start == end) {
                tree[node] = val;
            } else {
                int mid = (start + end) / 2;
                if (idx <= mid) {
                    update(2 * node, start, mid, idx, val);
                } else {
                    update(2 * node + 1, mid + 1, end, idx, val);
                }
                tree[node] = gcd(tree[2 * node], tree[2 * node + 1]);
            }
        }
        
        long long query(int node, int start, int end, int l, int r) {
            if (r < start || end < l) {
                return 0;
            }
            if (l <= start && end <= r) {
                return tree[node];
            }
            int mid = (start + end) / 2;
            return gcd(query(2 * node, start, mid, l, r),
                      query(2 * node + 1, mid + 1, end, l, r));
        }
        
    public:
        GCDSegmentTree(vector<int>& arr) {
            n = arr.size();
            tree.resize(4 * n);
            build(arr, 1, 0, n - 1);
        }
        
        void update(int idx, int val) {
            update(1, 0, n - 1, idx, val);
        }
        
        long long query(int l, int r) {
            return query(1, 0, n - 1, l, r);
        }
    };
    
    // 2D Segment Tree (Range Sum Queries on 2D array)
    class SegmentTree2D {
    private:
        vector<vector<long long>> tree;
        int n, m;
        
        void buildY(vector<vector<int>>& arr, int vx, int lx, int rx, int vy, int ly, int ry) {
            if (ly == ry) {
                if (lx == rx) {
                    tree[vx][vy] = arr[lx][ly];
                } else {
                    tree[vx][vy] = tree[2*vx][vy] + tree[2*vx+1][vy];
                }
            } else {
                int my = (ly + ry) / 2;
                buildY(arr, vx, lx, rx, 2*vy, ly, my);
                buildY(arr, vx, lx, rx, 2*vy+1, my+1, ry);
                tree[vx][vy] = tree[vx][2*vy] + tree[vx][2*vy+1];
            }
        }
        
        void buildX(vector<vector<int>>& arr, int vx, int lx, int rx) {
            if (lx != rx) {
                int mx = (lx + rx) / 2;
                buildX(arr, 2*vx, lx, mx);
                buildX(arr, 2*vx+1, mx+1, rx);
            }
            buildY(arr, vx, lx, rx, 1, 0, m-1);
        }
        
        void updateY(vector<vector<int>>& arr, int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int val) {
            if (ly == ry) {
                if (lx == rx) {
                    tree[vx][vy] = val;
                } else {
                    tree[vx][vy] = tree[2*vx][vy] + tree[2*vx+1][vy];
                }
            } else {
                int my = (ly + ry) / 2;
                if (y <= my) {
                    updateY(arr, vx, lx, rx, 2*vy, ly, my, x, y, val);
                } else {
                    updateY(arr, vx, lx, rx, 2*vy+1, my+1, ry, x, y, val);
                }
                tree[vx][vy] = tree[vx][2*vy] + tree[vx][2*vy+1];
            }
        }
        
        void updateX(vector<vector<int>>& arr, int vx, int lx, int rx, int x, int y, int val) {
            if (lx != rx) {
                int mx = (lx + rx) / 2;
                if (x <= mx) {
                    updateX(arr, 2*vx, lx, mx, x, y, val);
                } else {
                    updateX(arr, 2*vx+1, mx+1, rx, x, y, val);
                }
            }
            updateY(arr, vx, lx, rx, 1, 0, m-1, x, y, val);
        }
        
        long long queryY(int vx, int vy, int ly, int ry, int y1, int y2) {
            if (y1 > y2) return 0;
            if (y1 == ly && y2 == ry) {
                return tree[vx][vy];
            }
            int my = (ly + ry) / 2;
            return queryY(vx, 2*vy, ly, my, y1, min(y2, my)) +
                   queryY(vx, 2*vy+1, my+1, ry, max(y1, my+1), y2);
        }
        
        long long queryX(int vx, int lx, int rx, int x1, int x2, int y1, int y2) {
            if (x1 > x2) return 0;
            if (x1 == lx && x2 == rx) {
                return queryY(vx, 1, 0, m-1, y1, y2);
            }
            int mx = (lx + rx) / 2;
            return queryX(2*vx, lx, mx, x1, min(x2, mx), y1, y2) +
                   queryX(2*vx+1, mx+1, rx, max(x1, mx+1), x2, y1, y2);
        }
        
        vector<vector<int>> arr;
        
    public:
        SegmentTree2D(vector<vector<int>>& matrix) {
            arr = matrix;
            n = matrix.size();
            m = matrix[0].size();
            tree.resize(4 * n, vector<long long>(4 * m));
            buildX(arr, 1, 0, n-1);
        }
        
        void update(int x, int y, int val) {
            arr[x][y] = val;
            updateX(arr, 1, 0, n-1, x, y, val);
        }
        
        long long query(int x1, int y1, int x2, int y2) {
            return queryX(1, 0, n-1, x1, x2, y1, y2);
        }
    };
    
    // Segment Tree with Range Assignment and Range Sum
    class RangeAssignmentTree {
    private:
        vector<long long> tree, lazy;
        vector<bool> hasLazy;
        int n;
        
        void push(int node, int start, int end) {
            if (hasLazy[node]) {
                tree[node] = lazy[node] * (end - start + 1);
                if (start != end) {
                    lazy[2 * node] = lazy[node];
                    lazy[2 * node + 1] = lazy[node];
                    hasLazy[2 * node] = hasLazy[2 * node + 1] = true;
                }
                hasLazy[node] = false;
            }
        }
        
        void build(vector<int>& arr, int node, int start, int end) {
            if (start == end) {
                tree[node] = arr[start];
            } else {
                int mid = (start + end) / 2;
                build(arr, 2 * node, start, mid);
                build(arr, 2 * node + 1, mid + 1, end);
                tree[node] = tree[2 * node] + tree[2 * node + 1];
            }
        }
        
        void updateRange(int node, int start, int end, int l, int r, int val) {
            push(node, start, end);
            if (start > r || end < l) return;
            
            if (start >= l && end <= r) {
                lazy[node] = val;
                hasLazy[node] = true;
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
            if (start > r || end < l) return 0;
            push(node, start, end);
            
            if (start >= l && end <= r) {
                return tree[node];
            }
            
            int mid = (start + end) / 2;
            return queryRange(2 * node, start, mid, l, r) +
                   queryRange(2 * node + 1, mid + 1, end, l, r);
        }
        
    public:
        RangeAssignmentTree(vector<int>& arr) {
            n = arr.size();
            tree.resize(4 * n);
            lazy.resize(4 * n);
            hasLazy.resize(4 * n, false);
            build(arr, 1, 0, n - 1);
        }
        
        void updateRange(int l, int r, int val) {
            updateRange(1, 0, n - 1, l, r, val);
        }
        
        long long queryRange(int l, int r) {
            return queryRange(1, 0, n - 1, l, r);
        }
    };
    
    // Segment Tree with Range XOR updates
    class XORSegmentTree {
    private:
        vector<long long> tree, lazy;
        int n;
        
        void push(int node, int start, int end) {
            if (lazy[node] != 0) {
                tree[node] ^= lazy[node] * (end - start + 1);
                if (start != end) {
                    lazy[2 * node] ^= lazy[node];
                    lazy[2 * node + 1] ^= lazy[node];
                }
                lazy[node] = 0;
            }
        }
        
        void build(vector<int>& arr, int node, int start, int end) {
            if (start == end) {
                tree[node] = arr[start];
            } else {
                int mid = (start + end) / 2;
                build(arr, 2 * node, start, mid);
                build(arr, 2 * node + 1, mid + 1, end);
                tree[node] = tree[2 * node] ^ tree[2 * node + 1];
            }
        }
        
        void updateRange(int node, int start, int end, int l, int r, int val) {
            push(node, start, end);
            if (start > r || end < l) return;
            
            if (start >= l && end <= r) {
                lazy[node] ^= val;
                push(node, start, end);
                return;
            }
            
            int mid = (start + end) / 2;
            updateRange(2 * node, start, mid, l, r, val);
            updateRange(2 * node + 1, mid + 1, end, l, r, val);
            
            push(2 * node, start, mid);
            push(2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] ^ tree[2 * node + 1];
        }
        
        long long queryRange(int node, int start, int end, int l, int r) {
            if (start > r || end < l) return 0;
            push(node, start, end);
            
            if (start >= l && end <= r) {
                return tree[node];
            }
            
            int mid = (start + end) / 2;
            return queryRange(2 * node, start, mid, l, r) ^
                   queryRange(2 * node + 1, mid + 1, end, l, r);
        }
        
    public:
        XORSegmentTree(vector<int>& arr) {
            n = arr.size();
            tree.resize(4 * n);
            lazy.resize(4 * n, 0);
            build(arr, 1, 0, n - 1);
        }
        
        void updateRange(int l, int r, int val) {
            updateRange(1, 0, n - 1, l, r, val);
        }
        
        long long queryRange(int l, int r) {
            return queryRange(1, 0, n - 1, l, r);
        }
    };
}

// Example usage:
/*
int main() {
    using namespace AdvancedSegmentTrees;
    
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    
    // Max Segment Tree
    MaxSegmentTree maxST(arr);
    cout << "Max in range [1, 4]: " << maxST.query(1, 4) << endl;
    
    // GCD Segment Tree
    GCDSegmentTree gcdST(arr);
    cout << "GCD in range [1, 4]: " << gcdST.query(1, 4) << endl;
    
    // 2D Segment Tree
    vector<vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    SegmentTree2D st2d(matrix);
    cout << "Sum in rectangle (0,0) to (1,1): " << st2d.query(0, 0, 1, 1) << endl;
    
    return 0;
}
*/