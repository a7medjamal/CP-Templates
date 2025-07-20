#include <bits/stdc++.h>
using namespace std;

// Mathematical utilities for competitive programming
namespace MathUtils {
    
    // Basic mathematical functions
    template<typename T>
    T gcd(T a, T b) {
        return b == 0 ? a : gcd(b, a % b);
    }
    
    template<typename T>
    T lcm(T a, T b) {
        return a / gcd(a, b) * b;
    }
    
    template<typename T>
    T power(T base, T exp, T mod = 1e9 + 7) {
        T result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }
    
    // Extended Euclidean Algorithm
    template<typename T>
    T extendedGCD(T a, T b, T& x, T& y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        T x1, y1;
        T gcd = extendedGCD(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return gcd;
    }
    
    // Check if number is prime
    bool isPrime(long long n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        
        for (long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }
    
    // Sieve of Eratosthenes
    vector<bool> sieve(int n) {
        vector<bool> is_prime(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        
        for (int i = 2; i * i <= n; i++) {
            if (is_prime[i]) {
                for (int j = i * i; j <= n; j += i) {
                    is_prime[j] = false;
                }
            }
        }
        
        return is_prime;
    }
    
    // Generate all primes up to n
    vector<int> generatePrimes(int n) {
        vector<bool> is_prime = sieve(n);
        vector<int> primes;
        
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) {
                primes.push_back(i);
            }
        }
        
        return primes;
    }
    
    // Generate all divisors
    vector<long long> getDivisors(long long n) {
        vector<long long> divisors;
        for (long long i = 1; i * i <= n; i++) {
            if (n % i == 0) {
                divisors.push_back(i);
                if (i != n / i) {
                    divisors.push_back(n / i);
                }
            }
        }
        sort(divisors.begin(), divisors.end());
        return divisors;
    }
    
    // Prime factorization
    vector<pair<long long, int>> primeFactors(long long n) {
        vector<pair<long long, int>> factors;
        
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                int count = 0;
                while (n % i == 0) {
                    n /= i;
                    count++;
                }
                factors.push_back({i, count});
            }
        }
        
        if (n > 1) factors.push_back({n, 1});
        return factors;
    }
    
    // Modular arithmetic
    const long long MOD = 1e9 + 7;
    
    long long addMod(long long a, long long b, long long mod = MOD) {
        return ((a % mod) + (b % mod)) % mod;
    }
    
    long long subMod(long long a, long long b, long long mod = MOD) {
        return ((a % mod) - (b % mod) + mod) % mod;
    }
    
    long long mulMod(long long a, long long b, long long mod = MOD) {
        return ((a % mod) * (b % mod)) % mod;
    }
    
    long long powMod(long long base, long long exp, long long mod = MOD) {
        return power(base, exp, mod);
    }
    
    long long invMod(long long a, long long mod = MOD) {
        return powMod(a, mod - 2, mod); // Fermat's little theorem
    }
    
    long long divMod(long long a, long long b, long long mod = MOD) {
        return mulMod(a, invMod(b, mod), mod);
    }
    
    // Combinatorics
    vector<long long> fact, inv_fact;
    
    void precomputeFactorials(int n, long long mod = MOD) {
        fact.resize(n + 1);
        inv_fact.resize(n + 1);
        
        fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = mulMod(fact[i - 1], i, mod);
        }
        
        inv_fact[n] = invMod(fact[n], mod);
        for (int i = n - 1; i >= 0; i--) {
            inv_fact[i] = mulMod(inv_fact[i + 1], i + 1, mod);
        }
    }
    
    long long nCr(int n, int r, long long mod = MOD) {
        if (r < 0 || r > n) return 0;
        return mulMod(fact[n], mulMod(inv_fact[r], inv_fact[n - r], mod), mod);
    }
    
    long long nPr(int n, int r, long long mod = MOD) {
        if (r < 0 || r > n) return 0;
        return mulMod(fact[n], inv_fact[n - r], mod);
    }
    
    // Number theory functions
    long long phi(long long n) { // Euler's totient function
        long long result = n;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0) n /= i;
                result -= result / i;
            }
        }
        if (n > 1) result -= result / n;
        return result;
    }
    
    // Check if number is perfect square
    bool isPerfectSquare(long long n) {
        if (n < 0) return false;
        long long root = sqrt(n);
        return root * root == n;
    }
    
    // Check if number is power of 2
    bool isPowerOfTwo(long long n) {
        return n > 0 && (n & (n - 1)) == 0;
    }
    
    // Next power of 2
    long long nextPowerOfTwo(long long n) {
        if (n <= 1) return 1;
        return 1LL << (64 - __builtin_clzll(n - 1));
    }
    
    // Bit manipulation utilities
    int popcount(long long n) {
        return __builtin_popcountll(n);
    }
    
    int trailingZeros(long long n) {
        return __builtin_ctzll(n);
    }
    
    int leadingZeros(long long n) {
        return __builtin_clzll(n);
    }
    
    // Matrix operations
    template<typename T>
    vector<vector<T>> matrixMultiply(const vector<vector<T>>& A, const vector<vector<T>>& B, T mod = MOD) {
        int n = A.size(), m = B[0].size(), p = B.size();
        vector<vector<T>> C(n, vector<T>(m, 0));
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < p; k++) {
                    C[i][j] = addMod(C[i][j], mulMod(A[i][k], B[k][j], mod), mod);
                }
            }
        }
        
        return C;
    }
    
    template<typename T>
    vector<vector<T>> matrixPower(vector<vector<T>> base, long long exp, T mod = MOD) {
        int n = base.size();
        vector<vector<T>> result(n, vector<T>(n, 0));
        
        // Initialize identity matrix
        for (int i = 0; i < n; i++) {
            result[i][i] = 1;
        }
        
        while (exp > 0) {
            if (exp & 1) {
                result = matrixMultiply(result, base, mod);
            }
            base = matrixMultiply(base, base, mod);
            exp >>= 1;
        }
        
        return result;
    }
}

// Example usage:
/*
int main() {
    // Basic math operations
    cout << "GCD(12, 18): " << MathUtils::gcd(12, 18) << endl;
    cout << "LCM(12, 18): " << MathUtils::lcm(12, 18) << endl;
    cout << "2^10 mod 1000000007: " << MathUtils::power(2LL, 10LL) << endl;
    
    // Prime checking
    cout << "Is 17 prime? " << MathUtils::isPrime(17) << endl;
    
    // Generate primes up to 100
    vector<int> primes = MathUtils::generatePrimes(100);
    
    // Combinatorics
    MathUtils::precomputeFactorials(1000);
    cout << "C(10, 3): " << MathUtils::nCr(10, 3) << endl;
    
    return 0;
}
*/