#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 5;

// Prime-related algorithms
namespace PrimeAlgorithms {
    
    // Check if number is prime (deterministic for n < 9,080,191)
    bool isPrime(long long n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        
        for (long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }
    
    // Miller-Rabin primality test
    long long mulmod(long long a, long long b, long long mod) {
        return (__int128)a * b % mod;
    }
    
    long long powmod(long long base, long long exp, long long mod) {
        long long result = 1;
        while (exp > 0) {
            if (exp % 2 == 1) result = mulmod(result, base, mod);
            base = mulmod(base, base, mod);
            exp /= 2;
        }
        return result;
    }
    
    bool millerRabin(long long n, long long a) {
        if (n <= 1 || a <= 1 || a >= n) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        long long d = n - 1;
        int r = 0;
        while (d % 2 == 0) {
            d /= 2;
            r++;
        }
        
        long long x = powmod(a, d, n);
        if (x == 1 || x == n - 1) return true;
        
        for (int i = 0; i < r - 1; i++) {
            x = mulmod(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    }
    
    bool isPrimeMR(long long n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0) return false;
        
        vector<long long> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        for (long long a : witnesses) {
            if (a >= n) break;
            if (!millerRabin(n, a)) return false;
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
    
    // Linear sieve (more efficient)
    vector<int> linearSieve(int n) {
        vector<int> spf(n + 1); // smallest prime factor
        vector<int> primes;
        
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            
            for (int j = 0; j < (int)primes.size() && primes[j] <= spf[i] && i * primes[j] <= n; j++) {
                spf[i * primes[j]] = primes[j];
            }
        }
        return primes;
    }
    
    // Get all primes up to n
    vector<int> getPrimes(int n) {
        vector<bool> is_prime = sieve(n);
        vector<int> primes;
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) primes.push_back(i);
        }
        return primes;
    }
    
    // Prime factorization
    vector<pair<long long, int>> factorize(long long n) {
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
    
    // Fast prime factorization using precomputed smallest prime factors
    vector<pair<int, int>> factorizeWithSPF(int n, const vector<int>& spf) {
        vector<pair<int, int>> factors;
        
        while (n > 1) {
            int p = spf[n];
            int count = 0;
            while (n % p == 0) {
                n /= p;
                count++;
            }
            factors.push_back({p, count});
        }
        return factors;
    }
    
    // Pollard's rho algorithm for factorization
    long long pollardRho(long long n) {
        if (n % 2 == 0) return 2;
        
        auto f = [&](long long x) { return (mulmod(x, x, n) + 1) % n; };
        
        long long x = 2, y = 2, d = 1;
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = __gcd(abs(x - y), n);
        }
        return d;
    }
    
    // Complete factorization using Pollard's rho
    vector<long long> factorizeRho(long long n) {
        vector<long long> factors;
        
        function<void(long long)> factor = [&](long long num) {
            if (num == 1) return;
            if (isPrimeMR(num)) {
                factors.push_back(num);
                return;
            }
            
            long long d = pollardRho(num);
            factor(d);
            factor(num / d);
        };
        
        factor(n);
        sort(factors.begin(), factors.end());
        return factors;
    }
    
    // Segmented sieve for large ranges
    vector<bool> segmentedSieve(long long L, long long R) {
        long long lim = sqrt(R);
        vector<bool> mark(lim + 1, false);
        vector<long long> primes;
        
        for (long long i = 2; i <= lim; ++i) {
            if (!mark[i]) {
                primes.push_back(i);
                for (long long j = i * i; j <= lim; j += i)
                    mark[j] = true;
            }
        }
        
        vector<bool> isPrime(R - L + 1, true);
        for (long long i : primes) {
            for (long long j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
                isPrime[j - L] = false;
        }
        
        if (L == 1) isPrime[0] = false;
        return isPrime;
    }
    
    // Count primes up to n using Meissel-Lehmer algorithm (approximation)
    long long countPrimes(long long n) {
        if (n < 2) return 0;
        
        long long sqrtn = sqrt(n);
        vector<bool> is_prime = sieve(sqrtn);
        vector<int> primes;
        for (int i = 2; i <= sqrtn; i++) {
            if (is_prime[i]) primes.push_back(i);
        }
        
        // Simple counting for demonstration
        long long count = 0;
        vector<bool> segment = segmentedSieve(2, n);
        for (bool prime : segment) {
            if (prime) count++;
        }
        return count;
    }
    
    // Next prime after n
    long long nextPrime(long long n) {
        if (n < 2) return 2;
        n++;
        while (!isPrimeMR(n)) n++;
        return n;
    }
    
    // Previous prime before n
    long long prevPrime(long long n) {
        if (n <= 2) return -1;
        n--;
        while (n >= 2 && !isPrimeMR(n)) n--;
        return n >= 2 ? n : -1;
    }
    
    // Check if n is a prime power (p^k for some prime p and k >= 1)
    pair<long long, int> isPrimePower(long long n) {
        if (n <= 1) return {-1, 0};
        
        for (int k = 2; k <= 60; k++) { // log2(10^18) ≈ 60
            long long root = round(pow(n, 1.0 / k));
            
            // Check root and root±1 due to floating point precision
            for (long long r = max(2LL, root - 1); r <= root + 1; r++) {
                long long power = 1;
                for (int i = 0; i < k; i++) {
                    if (power > n / r) break; // Prevent overflow
                    power *= r;
                }
                if (power == n && isPrimeMR(r)) {
                    return {r, k};
                }
            }
        }
        
        if (isPrimeMR(n)) return {n, 1};
        return {-1, 0};
    }
    
    // Generate twin primes up to n
    vector<pair<int, int>> twinPrimes(int n) {
        vector<bool> is_prime = sieve(n);
        vector<pair<int, int>> twins;
        
        for (int i = 3; i <= n - 2; i++) {
            if (is_prime[i] && is_prime[i + 2]) {
                twins.push_back({i, i + 2});
            }
        }
        return twins;
    }
    
    // Check if n can be expressed as sum of two primes (Goldbach's conjecture)
    pair<int, int> goldbach(int n) {
        if (n <= 2 || n % 2 != 0) return {-1, -1};
        
        vector<bool> is_prime = sieve(n);
        for (int i = 2; i <= n / 2; i++) {
            if (is_prime[i] && is_prime[n - i]) {
                return {i, n - i};
            }
        }
        return {-1, -1};
    }
}

// Example usage:
/*
int main() {
    using namespace PrimeAlgorithms;
    
    cout << "Is 17 prime? " << (isPrime(17) ? "Yes" : "No") << endl;
    cout << "Is 1000000007 prime? " << (isPrimeMR(1000000007) ? "Yes" : "No") << endl;
    
    auto factors = factorize(60);
    cout << "Prime factorization of 60: ";
    for (auto& p : factors) {
        cout << p.first << "^" << p.second << " ";
    }
    cout << endl;
    
    auto primes = getPrimes(30);
    cout << "Primes up to 30: ";
    for (int p : primes) {
        cout << p << " ";
    }
    cout << endl;
    
    auto twins = twinPrimes(30);
    cout << "Twin primes up to 30: ";
    for (auto& twin : twins) {
        cout << "(" << twin.first << ", " << twin.second << ") ";
    }
    cout << endl;
    
    return 0;
}
*/