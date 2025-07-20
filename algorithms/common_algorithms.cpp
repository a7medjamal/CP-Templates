#include <bits/stdc++.h>
using namespace std;

// Common Utility Algorithms
// Note: Searching algorithms moved to searching_algorithms.cpp
// Note: Sorting algorithms moved to sorting_algorithms.cpp  
// Note: Sequence algorithms moved to sequence_algorithms.cpp
namespace CommonAlgorithms {
    
    // Coordinate Compression
    vector<int> coordinateCompression(vector<int> arr) {
        vector<int> sorted_arr = arr;
        sort(sorted_arr.begin(), sorted_arr.end());
        sorted_arr.erase(unique(sorted_arr.begin(), sorted_arr.end()), sorted_arr.end());
        
        for (int& x : arr) {
            x = lower_bound(sorted_arr.begin(), sorted_arr.end(), x) - sorted_arr.begin();
        }
        
        return arr;
    }
    
    // Discretization with mapping
    pair<vector<int>, vector<int>> discretize(const vector<int>& arr) {
        vector<int> sorted_unique = arr;
        sort(sorted_unique.begin(), sorted_unique.end());
        sorted_unique.erase(unique(sorted_unique.begin(), sorted_unique.end()), sorted_unique.end());
        
        vector<int> compressed(arr.size());
        for (int i = 0; i < (int)arr.size(); i++) {
            compressed[i] = lower_bound(sorted_unique.begin(), sorted_unique.end(), arr[i]) - sorted_unique.begin();
        }
        
        return {compressed, sorted_unique};
    }
    
    // Fast Power (Exponentiation by Squaring)
    long long fastPower(long long base, long long exp, long long mod = 1e9 + 7) {
        long long result = 1;
        base %= mod;
        
        while (exp > 0) {
            if (exp & 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exp >>= 1;
        }
        
        return result;
    }
    
    // Modular Inverse
    long long modInverse(long long a, long long mod) {
        return fastPower(a, mod - 2, mod);
    }
    
    // GCD and LCM
    long long gcd(long long a, long long b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }
    
    long long lcm(long long a, long long b) {
        return a / gcd(a, b) * b;
    }
    
    // Extended Euclidean Algorithm
    long long extendedGCD(long long a, long long b, long long& x, long long& y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        
        long long x1, y1;
        long long gcd = extendedGCD(b, a % b, x1, y1);
        
        x = y1;
        y = x1 - (a / b) * y1;
        
        return gcd;
    }
    
    // Bit Manipulation Utilities
    
    // Count set bits
    int popcount(long long x) {
        return __builtin_popcountll(x);
    }
    
    // Check if power of 2
    bool isPowerOfTwo(long long x) {
        return x > 0 && (x & (x - 1)) == 0;
    }
    
    // Next power of 2
    long long nextPowerOfTwo(long long x) {
        if (x <= 1) return 1;
        return 1LL << (64 - __builtin_clzll(x - 1));
    }
    
    // Get bit at position
    bool getBit(long long x, int pos) {
        return (x >> pos) & 1;
    }
    
    // Set bit at position
    long long setBit(long long x, int pos) {
        return x | (1LL << pos);
    }
    
    // Clear bit at position
    long long clearBit(long long x, int pos) {
        return x & ~(1LL << pos);
    }
    
    // Toggle bit at position
    long long toggleBit(long long x, int pos) {
        return x ^ (1LL << pos);
    }
    
    // Random Number Generation
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    int randomInt(int l, int r) {
        return uniform_int_distribution<int>(l, r)(rng);
    }
    
    long long randomLong(long long l, long long r) {
        return uniform_int_distribution<long long>(l, r)(rng);
    }
    
    double randomDouble(double l, double r) {
        return uniform_real_distribution<double>(l, r)(rng);
    }
    
    // Shuffle array
    template<typename T>
    void shuffleArray(vector<T>& arr) {
        shuffle(arr.begin(), arr.end(), rng);
    }
    
    // Time and Date Utilities
    
    // Check if leap year
    bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
    
    // Days in month
    int daysInMonth(int month, int year) {
        vector<int> days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear(year)) return 29;
        return days[month - 1];
    }
    
    // Day of week (0 = Sunday, 1 = Monday, ...)
    int dayOfWeek(int day, int month, int year) {
        if (month < 3) {
            month += 12;
            year--;
        }
        
        int k = year % 100;
        int j = year / 100;
        
        int h = (day + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
        return (h + 5) % 7; // Convert to 0=Sunday format
    }
    
    // String Utilities
    
    // Split string by delimiter
    vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    // Join strings with delimiter
    string join(const vector<string>& strings, const string& delimiter) {
        if (strings.empty()) return "";
        
        string result = strings[0];
        for (int i = 1; i < (int)strings.size(); i++) {
            result += delimiter + strings[i];
        }
        
        return result;
    }
    
    // Trim whitespace
    string trim(const string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        if (start == string::npos) return "";
        
        size_t end = str.find_last_not_of(" \t\n\r");
        return str.substr(start, end - start + 1);
    }
    
    // Convert to lowercase
    string toLowerCase(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
    
    // Convert to uppercase
    string toUpperCase(string str) {
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
    
    // Check if string is palindrome
    bool isPalindrome(const string& str) {
        int left = 0, right = str.length() - 1;
        
        while (left < right) {
            if (str[left] != str[right]) return false;
            left++;
            right--;
        }
        
        return true;
    }
    
    // Array/Vector Utilities
    
    // Rotate array left by k positions
    template<typename T>
    void rotateLeft(vector<T>& arr, int k) {
        int n = arr.size();
        k %= n;
        reverse(arr.begin(), arr.begin() + k);
        reverse(arr.begin() + k, arr.end());
        reverse(arr.begin(), arr.end());
    }
    
    // Rotate array right by k positions
    template<typename T>
    void rotateRight(vector<T>& arr, int k) {
        int n = arr.size();
        k %= n;
        reverse(arr.begin(), arr.end());
        reverse(arr.begin(), arr.begin() + k);
        reverse(arr.begin() + k, arr.end());
    }
    
    // Find all subarrays with given sum
    vector<pair<int, int>> findSubarraysWithSum(const vector<int>& arr, int targetSum) {
        vector<pair<int, int>> result;
        unordered_map<int, vector<int>> prefixSumMap;
        prefixSumMap[0].push_back(-1);
        
        int prefixSum = 0;
        for (int i = 0; i < (int)arr.size(); i++) {
            prefixSum += arr[i];
            
            if (prefixSumMap.count(prefixSum - targetSum)) {
                for (int startIdx : prefixSumMap[prefixSum - targetSum]) {
                    result.push_back({startIdx + 1, i});
                }
            }
            
            prefixSumMap[prefixSum].push_back(i);
        }
        
        return result;
    }
    
    // Check if array is sorted
    template<typename T>
    bool isSorted(const vector<T>& arr) {
        for (int i = 1; i < (int)arr.size(); i++) {
            if (arr[i] < arr[i-1]) return false;
        }
        return true;
    }
    
    // Remove duplicates from sorted array
    template<typename T>
    int removeDuplicates(vector<T>& arr) {
        if (arr.empty()) return 0;
        
        int writeIndex = 1;
        for (int i = 1; i < (int)arr.size(); i++) {
            if (arr[i] != arr[i-1]) {
                arr[writeIndex++] = arr[i];
            }
        }
        
        return writeIndex;
    }
}

// Example usage:
/*
int main() {
    using namespace CommonAlgorithms;
    
    vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // Coordinate compression
    vector<int> compressed = coordinateCompression(arr);
    cout << "Compressed array: ";
    for (int x : compressed) cout << x << " ";
    cout << endl;
    
    // Fast power
    cout << "2^10 mod 1000000007: " << fastPower(2, 10) << endl;
    
    // GCD and LCM
    cout << "GCD(12, 18): " << gcd(12, 18) << endl;
    cout << "LCM(12, 18): " << lcm(12, 18) << endl;
    
    // Bit manipulation
    cout << "Popcount of 15: " << popcount(15) << endl;
    cout << "Is 16 power of 2: " << isPowerOfTwo(16) << endl;
    
    // String utilities
    string text = "  Hello World  ";
    cout << "Trimmed: '" << trim(text) << "'" << endl;
    cout << "Lowercase: " << toLowerCase("HELLO") << endl;
    
    // Array utilities
    vector<int> test = {1, 2, 3, 4, 5};
    rotateLeft(test, 2);
    cout << "Rotated left by 2: ";
    for (int x : test) cout << x << " ";
    cout << endl;
    
    return 0;
}
*/