#include <bits/stdc++.h>
using namespace std;

namespace StringHashing {
    // Single Hash with Rolling Hash
    class SingleHash {
    private:
        static const int MOD = 1e9 + 7;
        static const int BASE = 31;
        
    public:
        vector<long long> hash, pow_base;
        string s;
        
        SingleHash(string str) : s(str) {
            int n = s.length();
            hash.resize(n + 1);
            pow_base.resize(n + 1);
            
            pow_base[0] = 1;
            for (int i = 1; i <= n; i++) {
                pow_base[i] = (pow_base[i-1] * BASE) % MOD;
            }
            
            for (int i = 0; i < n; i++) {
                hash[i+1] = (hash[i] * BASE + s[i]) % MOD;
            }
        }
        
        long long getHash(int l, int r) const {
            long long result = (hash[r+1] - hash[l] * pow_base[r-l+1] % MOD + MOD) % MOD;
            return result;
        }
        
        bool isEqual(int l1, int r1, int l2, int r2) {
            return getHash(l1, r1) == getHash(l2, r2);
        }
    };
    
    // Double Hash for better collision resistance
    class DoubleHash {
    private:
        static const int MOD1 = 1e9 + 7;
        static const int MOD2 = 1e9 + 9;
        static const int BASE1 = 31;
        static const int BASE2 = 37;
        
    public:
        vector<long long> hash1, hash2, pow1, pow2;
        string s;
        
        DoubleHash(string str) : s(str) {
            int n = s.length();
            hash1.resize(n + 1);
            hash2.resize(n + 1);
            pow1.resize(n + 1);
            pow2.resize(n + 1);
            
            pow1[0] = pow2[0] = 1;
            for (int i = 1; i <= n; i++) {
                pow1[i] = (pow1[i-1] * BASE1) % MOD1;
                pow2[i] = (pow2[i-1] * BASE2) % MOD2;
            }
            
            for (int i = 0; i < n; i++) {
                hash1[i+1] = (hash1[i] * BASE1 + s[i]) % MOD1;
                hash2[i+1] = (hash2[i] * BASE2 + s[i]) % MOD2;
            }
        }
        
        pair<long long, long long> getHash(int l, int r) const {
            long long h1 = (hash1[r+1] - hash1[l] * pow1[r-l+1] % MOD1 + MOD1) % MOD1;
            long long h2 = (hash2[r+1] - hash2[l] * pow2[r-l+1] % MOD2 + MOD2) % MOD2;
            return {h1, h2};
        }
        
        bool isEqual(int l1, int r1, int l2, int r2) {
            return getHash(l1, r1) == getHash(l2, r2);
        }
    };
    
    // Polynomial Rolling Hash
    class PolynomialHash {
    private:
        static const int MOD = 1e9 + 7;
        int base;
        
    public:
        vector<long long> hash, pow_base;
        string s;
        
        PolynomialHash(string str, int b = 31) : s(str), base(b) {
            int n = s.length();
            hash.resize(n + 1);
            pow_base.resize(n + 1);
            
            pow_base[0] = 1;
            for (int i = 1; i <= n; i++) {
                pow_base[i] = (pow_base[i-1] * base) % MOD;
            }
            
            for (int i = 0; i < n; i++) {
                hash[i+1] = (hash[i] * base + (s[i] - 'a' + 1)) % MOD;
            }
        }
        
        long long getHash(int l, int r) {
            long long result = (hash[r+1] - hash[l] * pow_base[r-l+1] % MOD + MOD) % MOD;
            return result;
        }
        
        long long getFullHash() const {
            return hash[s.length()];
        }
    };
    
    // Hash utilities
    namespace HashUtils {
        // Compute hash of a string
        long long computeHash(string s, int base = 31, int mod = 1e9 + 7) {
            long long hash_value = 0;
            long long pow_base = 1;
            
            for (char c : s) {
                hash_value = (hash_value + (c - 'a' + 1) * pow_base) % mod;
                pow_base = (pow_base * base) % mod;
            }
            
            return hash_value;
        }
        
        // Compare two substrings using hash
        bool compareSubstrings(const DoubleHash& h1, int l1, int r1,
                             const DoubleHash& h2, int l2, int r2) {
            return h1.getHash(l1, r1) == h2.getHash(l2, r2);
        }
        
        // Find all occurrences of pattern in text using rolling hash
        vector<int> findOccurrences(string text, string pattern) {
            vector<int> result;
            if (pattern.length() > text.length()) return result;
            
            DoubleHash textHash(text);
            DoubleHash patternHash(pattern);
            
            auto patternHashValue = patternHash.getHash(0, pattern.length() - 1);
            
            for (int i = 0; i <= (int)text.length() - (int)pattern.length(); i++) {
                if (textHash.getHash(i, i + pattern.length() - 1) == patternHashValue) {
                    result.push_back(i);
                }
            }
            
            return result;
        }
        
        // Longest common prefix using binary search and hashing
        int longestCommonPrefix(const DoubleHash& h1, int start1,
                              const DoubleHash& h2, int start2) {
            int left = 0, right = min((int)h1.s.length() - start1, (int)h2.s.length() - start2);
            int result = 0;
            
            while (left <= right) {
                int mid = (left + right) / 2;
                if (h1.getHash(start1, start1 + mid - 1) == h2.getHash(start2, start2 + mid - 1)) {
                    result = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            
            return result;
        }
        
        // Check if string is periodic using hash
        bool isPeriodic(string s, int period) {
            if (s.length() % period != 0) return false;
            
            DoubleHash hash(s);
            auto firstPeriodHash = hash.getHash(0, period - 1);
            
            for (int i = period; i < (int)s.length(); i += period) {
                if (hash.getHash(i, i + period - 1) != firstPeriodHash) {
                    return false;
                }
            }
            
            return true;
        }
        
        // Find smallest period of string
        int smallestPeriod(string s) {
            for (int period = 1; period <= (int)s.length(); period++) {
                if (isPeriodic(s, period)) {
                    return period;
                }
            }
            return s.length();
        }
    }
    
    // Advanced hashing techniques
    namespace AdvancedHashing {
        // Zobrist hashing for 2D grids
        class ZobristHash {
        private:
            vector<vector<long long>> zobrist;
            int rows, cols;
            
        public:
            ZobristHash(int r, int c) : rows(r), cols(c) {
                zobrist.resize(rows, vector<long long>(cols));
                random_device rd;
                mt19937_64 gen(rd());
                
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        zobrist[i][j] = gen();
                    }
                }
            }
            
            long long computeHash(const vector<vector<int>>& grid) {
                long long hash = 0;
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (grid[i][j]) {
                            hash ^= zobrist[i][j];
                        }
                    }
                }
                return hash;
            }
        };
        
        // Cyclic hash for rotation-invariant hashing
        long long cyclicHash(string s) {
            vector<long long> hashes;
            DoubleHash hash(s + s);
            
            for (int i = 0; i < (int)s.length(); i++) {
                hashes.push_back(hash.getHash(i, i + s.length() - 1).first);
            }
            
            sort(hashes.begin(), hashes.end());
            return hashes[0]; // Return minimum rotation hash
        }
        
        // Multiset hash (order-independent)
        long long multisetHash(vector<string>& strings) {
            long long result = 0;
            for (string& s : strings) {
                DoubleHash hash(s);
                result ^= hash.getHash(0, s.length() - 1).first;
            }
            return result;
        }
    }
}

// Example usage:
/*
int main() {
    using namespace StringHashing;
    
    string text = "abcabcabc";
    string pattern = "abc";
    
    // Single hash
    SingleHash singleHash(text);
    cout << "Hash of substring [0, 2]: " << singleHash.getHash(0, 2) << endl;
    
    // Double hash
    DoubleHash doubleHash(text);
    auto hashPair = doubleHash.getHash(0, 2);
    cout << "Double hash of substring [0, 2]: (" << hashPair.first << ", " << hashPair.second << ")" << endl;
    
    // Find occurrences
    auto occurrences = HashUtils::findOccurrences(text, pattern);
    cout << "Pattern occurrences: ";
    for (int pos : occurrences) {
        cout << pos << " ";
    }
    cout << endl;
    
    // Check periodicity
    cout << "Is periodic with period 3: " << HashUtils::isPeriodic(text, 3) << endl;
    cout << "Smallest period: " << HashUtils::smallestPeriod(text) << endl;
    
    return 0;
}
*/