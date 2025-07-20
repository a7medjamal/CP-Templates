#include <bits/stdc++.h>
using namespace std;

// Input/Output utilities for competitive programming
namespace IOUtils {
    
    // Fast I/O
    void fastIO() {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }
    
    // Debug utilities
    template<typename T>
    void debug(const T& x) {
        cerr << x;
    }
    
    template<typename T, typename... Args>
    void debug(const T& x, const Args&... args) {
        cerr << x << ", ";
        debug(args...);
    }
    
    #ifdef LOCAL
    #define dbg(...) cerr << "[" << #__VA_ARGS__ << "]: ", debug(__VA_ARGS__), cerr << endl
    #else
    #define dbg(...)
    #endif
    
    // Print utilities
    template<typename T>
    void print(const vector<T>& v, const string& sep = " ", const string& end = "\n") {
        for (int i = 0; i < (int)v.size(); i++) {
            cout << v[i] << (i == v.size() - 1 ? end : sep);
        }
    }
    
    template<typename T>
    void print2D(const vector<vector<T>>& matrix) {
        for (const auto& row : matrix) {
            print(row);
        }
    }
    
    template<typename T>
    void printPairs(const vector<pair<T, T>>& pairs) {
        for (const auto& p : pairs) {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << "\n";
    }
    
    // Input utilities
    template<typename T>
    vector<T> readVector(int n) {
        vector<T> v(n);
        for (int i = 0; i < n; i++) {
            cin >> v[i];
        }
        return v;
    }
    
    template<typename T>
    vector<vector<T>> readMatrix(int rows, int cols) {
        vector<vector<T>> matrix(rows, vector<T>(cols));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cin >> matrix[i][j];
            }
        }
        return matrix;
    }
    
    template<typename T>
    vector<pair<T, T>> readPairs(int n) {
        vector<pair<T, T>> pairs(n);
        for (int i = 0; i < n; i++) {
            cin >> pairs[i].first >> pairs[i].second;
        }
        return pairs;
    }
    
    // Read graph as adjacency list
    vector<vector<int>> readGraph(int n, int m, bool directed = false) {
        vector<vector<int>> graph(n + 1);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            graph[u].push_back(v);
            if (!directed) {
                graph[v].push_back(u);
            }
        }
        return graph;
    }
    
    // Read weighted graph
    vector<vector<pair<int, int>>> readWeightedGraph(int n, int m, bool directed = false) {
        vector<vector<pair<int, int>>> graph(n + 1);
        for (int i = 0; i < m; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            graph[u].push_back({v, w});
            if (!directed) {
                graph[v].push_back({u, w});
            }
        }
        return graph;
    }
    
    // String input utilities
    vector<string> readStrings(int n) {
        vector<string> strings(n);
        for (int i = 0; i < n; i++) {
            cin >> strings[i];
        }
        return strings;
    }
    
    // Grid input
    vector<string> readGrid(int rows) {
        vector<string> grid(rows);
        for (int i = 0; i < rows; i++) {
            cin >> grid[i];
        }
        return grid;
    }
    
    // Output formatting
    void printYesNo(bool condition) {
        cout << (condition ? "YES" : "NO") << "\n";
    }
    
    void printCase(int caseNum, const string& result) {
        cout << "Case #" << caseNum << ": " << result << "\n";
    }
    
    template<typename T>
    void printCase(int caseNum, const T& result) {
        cout << "Case #" << caseNum << ": " << result << "\n";
    }
}

// Example usage:
/*
int main() {
    IOUtils::fastIO();
    
    int n;
    cin >> n;
    
    vector<int> arr = IOUtils::readVector<int>(n);
    IOUtils::print(arr);
    
    vector<vector<int>> matrix = IOUtils::readMatrix<int>(3, 3);
    IOUtils::print2D(matrix);
    
    vector<vector<int>> graph = IOUtils::readGraph(n, n-1);
    
    IOUtils::printYesNo(true);
    IOUtils::printCase(1, "POSSIBLE");
    
    return 0;
}
*/