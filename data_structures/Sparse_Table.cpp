template<typename T, class CMP = function<T(const T &, const T &)>> 
struct Sparse_Table {
    int n;
    CMP merge;
    vector<vector<T>> sp;

    Sparse_Table(const vector<T>& arr, const CMP &F) : merge(F) {
        n = arr.size();
        int maxLog = 32 - __builtin_clz(n);
        sp.resize(maxLog);
        sp[0] = arr;
        for (int lg = 1; lg < maxLog; ++lg) {
            sp[lg].resize(n - (1 << lg) + 1);
            for (int i = 0; i + (1 << lg) <= n; ++i) {
                sp[lg][i] = merge(sp[lg - 1][i], sp[lg - 1][i + (1 << (lg - 1))]);
            }
        }
    }

    T query(int l, int r) {
        int lg = __lg(r - l + 1);
        return merge(sp[lg][l], sp[lg][r - (1 << lg) + 1]);
    }
};
