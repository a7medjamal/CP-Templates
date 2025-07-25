// Lazy Segment Tree works up to large ranges [0, 1e18]
// * range_add(l, r, val): add 'val' to range [l, r]
// * query(l, r): returns {min, max} in range [l, r]

class LazySparseSagara {
private:
    struct Node {
        int mx = 0, mn = 0;
        int lazy = 0;
        Node *left = nullptr;
        Node *right = nullptr;
    };
    Node *root = new Node;
    const int n;

    int combMx(int a, int b) { return max(a, b); }
    int combMn(int a, int b) { return min(a, b); }

    void apply(Node *cur, int len, int val) {
            (cur->lazy) += val;
            (cur->mx) += val;
            (cur->mn) += val;
    }

    void push_down(Node *cur, int l, int r) {
        if ((cur->left) == nullptr) { (cur->left) = new Node; }
        if ((cur->right) == nullptr) { (cur->right) = new Node; }
        int m = (l + r) / 2;
        apply(cur->left, m - l + 1, cur->lazy);
        apply(cur->right, r - m, cur->lazy);
        cur->lazy = 0;
    }

    void range_add(Node *cur, int l, int r, int ql, int qr, int val) {
        if (qr < l || ql > r) { return; }
        if (ql <= l && r <= qr) {
            apply(cur, r - l + 1, val);
        } else {
            push_down(cur, l, r);
            int m = (l + r) / 2;
            range_add(cur->left, l, m, ql, qr, val);
            range_add(cur->right, m + 1, r, ql, qr, val);
            (cur->mx) = combMx((cur->left)->mx, (cur->right)->mx);
            (cur->mn) = combMn((cur->left)->mn, (cur->right)->mn);
        }
    }

    int range_min(Node *cur, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) { return 3e18; }
        if (ql <= l && r <= qr) { return cur->mn; }
        push_down(cur, l, r);
        int m = (l + r) / 2;
        return combMn(range_min(cur->left, l, m, ql, qr),
                    range_min(cur->right, m + 1, r, ql, qr));
    }
    
    int range_max(Node *cur, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) { return 0; }
        if (ql <= l && r <= qr) { return cur->mx; }
        push_down(cur, l, r);
        int m = (l + r) / 2;
        return combMx(range_max(cur->left, l, m, ql, qr),
                    range_max(cur->right, m + 1, r, ql, qr));
    }

public:
    LazySparseSagara(int n) : n(n) {}

    void range_add(int ql, int qr, int val) { range_add(root, 0, n - 1, ql, qr, val); }

    pair<int, int> query(int ql, int qr) { return {range_min(root, 0, n - 1, ql, qr), range_max(root, 0, n - 1, ql, qr)}; }
};
