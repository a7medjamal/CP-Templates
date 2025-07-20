#include <bits/stdc++.h>
using namespace std;

// Convex Hull Trick / CHT for DP optimization
class ConvexHullTrick {
public:
    struct Line {
        long long m, b;
        int id;
        
        Line(long long m = 0, long long b = 0, int id = 0) : m(m), b(b), id(id) {}
        
        long long eval(long long x) {
            return m * x + b;
        }
        
        bool bad(const Line& l1, const Line& l2) {
            return (b - l1.b) * (l1.m - l2.m) <= (l2.b - l1.b) * (l1.m - m);
        }
    };
    
    deque<Line> lines;
    
    void addLine(long long m, long long b, int id = 0) {
        Line newLine(m, b, id);
        
        while (lines.size() >= 2 && 
               newLine.bad(lines[lines.size()-2], lines[lines.size()-1])) {
            lines.pop_back();
        }
        
        lines.push_back(newLine);
    }
    
    pair<long long, int> query(long long x) {
        while (lines.size() >= 2 && 
               lines[0].eval(x) >= lines[1].eval(x)) {
            lines.pop_front();
        }
        
        return {lines[0].eval(x), lines[0].id};
    }
    
    void clear() {
        lines.clear();
    }
    
    // For maximum instead of minimum
    void addLineMax(long long m, long long b, int id = 0) {
        addLine(-m, -b, id);
    }
    
    pair<long long, int> queryMax(long long x) {
        auto result = query(x);
        return {-result.first, result.second};
    }
};

// Li Chao Tree for dynamic CHT
class LiChaoTree {
public:
    struct Line {
        long long m, b;
        int id;
        
        Line(long long m = 0, long long b = LLONG_MIN, int id = -1) : m(m), b(b), id(id) {}
        
        long long eval(long long x) {
            return m * x + b;
        }
    };
    
    vector<Line> tree;
    int n;
    long long minX, maxX;
    
    LiChaoTree(long long minX, long long maxX) : minX(minX), maxX(maxX) {
        n = 1;
        while (n < maxX - minX + 1) n *= 2;
        tree.assign(4 * n, Line());
    }
    
    void update(int node, int start, int end, Line line) {
        if (start == end) {
            if (line.eval(start + minX) < tree[node].eval(start + minX)) {
                tree[node] = line;
            }
            return;
        }
        
        int mid = (start + end) / 2;
        long long midX = mid + minX;
        
        if (line.eval(midX) < tree[node].eval(midX)) {
            swap(line, tree[node]);
        }
        
        if (line.m < tree[node].m) {
            update(2 * node + 1, mid + 1, end, line);
        } else {
            update(2 * node, start, mid, line);
        }
    }
    
    long long query(int node, int start, int end, long long x) {
        if (start == end) {
            return tree[node].eval(x);
        }
        
        int mid = (start + end) / 2;
        long long result = tree[node].eval(x);
        
        if (x <= mid + minX) {
            result = min(result, query(2 * node, start, mid, x));
        } else {
            result = min(result, query(2 * node + 1, mid + 1, end, x));
        }
        
        return result;
    }
    
    void addLine(long long m, long long b, int id = 0) {
        update(1, 0, n - 1, Line(m, b, id));
    }
    
    long long query(long long x) {
        return query(1, 0, n - 1, x);
    }
};

// Example usage:
/*
int main() {
    ConvexHullTrick cht;
    cht.addLine(-2, 0, 0);
    cht.addLine(-1, 1, 1);
    auto result = cht.query(5);
    cout << "Best value: " << result.first << ", Line ID: " << result.second << endl;
    
    LiChaoTree lct(-1000, 1000);
    lct.addLine(1, 0);
    lct.addLine(-1, 100);
    cout << "Li Chao query at x=50: " << lct.query(50) << endl;
    
    return 0;
}
*/