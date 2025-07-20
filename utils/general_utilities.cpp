#include <bits/stdc++.h>
using namespace std;

// General utilities for competitive programming
namespace GeneralUtils {
    
    // Coordinate utilities
    struct Point {
        long long x, y;
        Point() : x(0), y(0) {}
        Point(long long x, long long y) : x(x), y(y) {}
        
        Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
        Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
        Point operator*(long long k) const { return Point(x * k, y * k); }
        bool operator<(const Point& p) const { return x < p.x || (x == p.x && y < p.y); }
        bool operator==(const Point& p) const { return x == p.x && y == p.y; }
        bool operator!=(const Point& p) const { return !(*this == p); }
        
        long long distSquared(const Point& p) const {
            return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
        }
        
        long long manhattanDist(const Point& p) const {
            return abs(x - p.x) + abs(y - p.y);
        }
        
        double euclideanDist(const Point& p) const {
            return sqrt(distSquared(p));
        }
        
        // Cross product (for orientation)
        long long cross(const Point& p) const {
            return x * p.y - y * p.x;
        }
        
        // Dot product
        long long dot(const Point& p) const {
            return x * p.x + y * p.y;
        }
    };
    
    // Direction vectors (4-directional and 8-directional)
    const vector<Point> dir4 = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    const vector<Point> dir8 = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
    
    // Check if point is within bounds
    bool inBounds(int x, int y, int rows, int cols) {
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }
    
    bool inBounds(const Point& p, int rows, int cols) {
        return inBounds(p.x, p.y, rows, cols);
    }
    
    // Check orientation of three points
    int orientation(const Point& p, const Point& q, const Point& r) {
        long long val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;  // Collinear
        return (val > 0) ? 1 : 2; // Clockwise or Counterclockwise
    }
    
    // String utilities
    string toLowerCase(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
    
    string toUpperCase(string s) {
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }
    
    vector<string> split(const string& s, char delimiter) {
        vector<string> tokens;
        stringstream ss(s);
        string token;
        
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    string join(const vector<string>& tokens, const string& delimiter) {
        if (tokens.empty()) return "";
        
        string result = tokens[0];
        for (int i = 1; i < tokens.size(); i++) {
            result += delimiter + tokens[i];
        }
        
        return result;
    }
    
    string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == string::npos) return "";
        
        size_t end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }
    
    bool isPalindrome(const string& s) {
        int left = 0, right = s.length() - 1;
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
    
    // Count occurrences of substring
    int countSubstring(const string& text, const string& pattern) {
        int count = 0;
        size_t pos = 0;
        while ((pos = text.find(pattern, pos)) != string::npos) {
            count++;
            pos += pattern.length();
        }
        return count;
    }
    
    // Array utilities
    template<typename T>
    T getMax(const vector<T>& v) {
        return *max_element(v.begin(), v.end());
    }
    
    template<typename T>
    T getMin(const vector<T>& v) {
        return *min_element(v.begin(), v.end());
    }
    
    template<typename T>
    T getSum(const vector<T>& v) {
        return accumulate(v.begin(), v.end(), T(0));
    }
    
    template<typename T>
    double getAverage(const vector<T>& v) {
        return v.empty() ? 0.0 : (double)getSum(v) / v.size();
    }
    
    template<typename T>
    void reverse(vector<T>& v) {
        reverse(v.begin(), v.end());
    }
    
    template<typename T>
    void sort(vector<T>& v) {
        sort(v.begin(), v.end());
    }
    
    template<typename T>
    void sortDesc(vector<T>& v) {
        sort(v.begin(), v.end(), greater<T>());
    }
    
    template<typename T>
    void removeDuplicates(vector<T>& v) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    }
    
    template<typename T>
    vector<T> getUnique(const vector<T>& v) {
        vector<T> result = v;
        removeDuplicates(result);
        return result;
    }
    
    // Rotate array left by k positions
    template<typename T>
    void rotateLeft(vector<T>& v, int k) {
        if (v.empty()) return;
        k %= v.size();
        reverse(v.begin(), v.begin() + k);
        reverse(v.begin() + k, v.end());
        reverse(v.begin(), v.end());
    }
    
    // Rotate array right by k positions
    template<typename T>
    void rotateRight(vector<T>& v, int k) {
        if (v.empty()) return;
        k %= v.size();
        rotateLeft(v, v.size() - k);
    }
    
    // Frequency counter
    template<typename T>
    map<T, int> getFrequency(const vector<T>& v) {
        map<T, int> freq;
        for (const T& x : v) {
            freq[x]++;
        }
        return freq;
    }
    
    template<typename T>
    unordered_map<T, int> getFrequencyFast(const vector<T>& v) {
        unordered_map<T, int> freq;
        for (const T& x : v) {
            freq[x]++;
        }
        return freq;
    }
    
    // Find mode (most frequent element)
    template<typename T>
    T getMode(const vector<T>& v) {
        auto freq = getFrequency(v);
        T mode = v[0];
        int maxFreq = 0;
        
        for (const auto& p : freq) {
            if (p.second > maxFreq) {
                maxFreq = p.second;
                mode = p.first;
            }
        }
        
        return mode;
    }
    
    // Random utilities
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    int randInt(int l, int r) {
        return uniform_int_distribution<int>(l, r)(rng);
    }
    
    long long randLong(long long l, long long r) {
        return uniform_int_distribution<long long>(l, r)(rng);
    }
    
    double randDouble(double l, double r) {
        return uniform_real_distribution<double>(l, r)(rng);
    }
    
    bool randBool() {
        return randInt(0, 1) == 1;
    }
    
    // Shuffle array
    template<typename T>
    void shuffle(vector<T>& v) {
        shuffle(v.begin(), v.end(), rng);
    }
    
    // Generate random array
    vector<int> generateRandomArray(int n, int minVal, int maxVal) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = randInt(minVal, maxVal);
        }
        return arr;
    }
    
    // Timer utility
    class Timer {
    private:
        chrono::high_resolution_clock::time_point start_time;
    public:
        Timer() {
            start_time = chrono::high_resolution_clock::now();
        }
        
        double elapsed() {
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            return duration.count() / 1000.0; // Return milliseconds
        }
        
        double elapsedSeconds() {
            return elapsed() / 1000.0;
        }
        
        void reset() {
            start_time = chrono::high_resolution_clock::now();
        }
        
        void printElapsed(const string& label = "Time") {
            cout << label << ": " << elapsed() << " ms" << endl;
        }
    };
    
    // Memory usage (approximate)
    void printMemoryUsage() {
        #ifdef LOCAL
        cerr << "Memory usage: " << (double)sizeof(int) * 1e6 / 1024 / 1024 << " MB" << endl;
        #endif
    }
    
    // Date and time utilities
    bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
    
    int daysInMonth(int month, int year) {
        vector<int> days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }
    
    // Convert seconds to human readable format
    string formatTime(long long seconds) {
        long long hours = seconds / 3600;
        seconds %= 3600;
        long long minutes = seconds / 60;
        seconds %= 60;
        
        return to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s";
    }
}

// Example usage:
/*
int main() {
    // Point operations
    GeneralUtils::Point p1(3, 4);
    GeneralUtils::Point p2(0, 0);
    cout << "Distance: " << p1.euclideanDist(p2) << endl;
    cout << "Manhattan distance: " << p1.manhattanDist(p2) << endl;
    
    // String utilities
    string text = "  Hello World  ";
    cout << "Trimmed: '" << GeneralUtils::trim(text) << "'" << endl;
    cout << "Lowercase: " << GeneralUtils::toLowerCase(text) << endl;
    
    // Array utilities
    vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    cout << "Max: " << GeneralUtils::getMax(arr) << endl;
    cout << "Sum: " << GeneralUtils::getSum(arr) << endl;
    cout << "Average: " << GeneralUtils::getAverage(arr) << endl;
    
    // Random utilities
    cout << "Random int [1, 10]: " << GeneralUtils::randInt(1, 10) << endl;
    
    // Timer
    GeneralUtils::Timer timer;
    // Some computation here
    timer.printElapsed("Computation time");
    
    return 0;
}
*/