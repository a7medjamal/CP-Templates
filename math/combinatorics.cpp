#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MAXN = 1e6 + 5;

// Combinatorics utilities
namespace Combinatorics {
    
    vector<long long> fact, inv_fact;
    bool initialized = false;
    
    // Fast modular exponentiation
    long long power(long long base, long long exp, long long mod = MOD) {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }
    
    // Modular inverse using Fermat's little theorem
    long long modinv(long long a, long long mod = MOD) {
        return power(a, mod - 2, mod);
    }
    
    // Precompute factorials and inverse factorials
    void init(int n = MAXN) {
        fact.resize(n);
        inv_fact.resize(n);
        
        fact[0] = 1;
        for (int i = 1; i < n; i++) {
            fact[i] = (fact[i - 1] * i) % MOD;
        }
        
        inv_fact[n - 1] = modinv(fact[n - 1]);
        for (int i = n - 2; i >= 0; i--) {
            inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % MOD;
        }
        
        initialized = true;
    }
    
    // Factorial
    long long factorial(int n) {
        if (!initialized) init();
        return fact[n];
    }
    
    // Binomial coefficient C(n, k)
    long long C(int n, int k) {
        if (!initialized) init();
        if (k > n || k < 0) return 0;
        return (fact[n] * inv_fact[k] % MOD) * inv_fact[n - k] % MOD;
    }
    
    // Permutation P(n, k) = n! / (n-k)!
    long long P(int n, int k) {
        if (!initialized) init();
        if (k > n || k < 0) return 0;
        return (fact[n] * inv_fact[n - k]) % MOD;
    }
    
    // Catalan number C_n = C(2n, n) / (n+1)
    long long catalan(int n) {
        if (!initialized) init();
        return (C(2 * n, n) * modinv(n + 1)) % MOD;
    }
    
    // Stirling numbers of the second kind S(n, k)
    // Number of ways to partition n objects into k non-empty subsets
    long long stirling2(int n, int k) {
        if (!initialized) init();
        if (k > n || k == 0) return 0;
        if (k == 1 || k == n) return 1;
        
        long long result = 0;
        for (int i = 0; i <= k; i++) {
            long long term = (C(k, i) * power(k - i, n)) % MOD;
            if (i % 2 == 0) {
                result = (result + term) % MOD;
            } else {
                result = (result - term + MOD) % MOD;
            }
        }
        
        return (result * inv_fact[k]) % MOD;
    }
    
    // Bell numbers B_n (number of partitions of n elements)
    vector<long long> bellNumbers(int n) {
        vector<vector<long long>> bell(n + 1, vector<long long>(n + 1));
        bell[0][0] = 1;
        
        for (int i = 1; i <= n; i++) {
            bell[i][0] = bell[i - 1][i - 1];
            for (int j = 1; j <= i; j++) {
                bell[i][j] = (bell[i - 1][j - 1] + bell[i][j - 1]) % MOD;
            }
        }
        
        vector<long long> result(n + 1);
        for (int i = 0; i <= n; i++) {
            result[i] = bell[i][0];
        }
        return result;
    }
    
    // Derangement numbers D_n (permutations with no fixed points)
    long long derangement(int n) {
        if (n == 0) return 1;
        if (n == 1) return 0;
        if (n == 2) return 1;
        
        long long dp0 = 1, dp1 = 0, dp2 = 1;
        for (int i = 3; i <= n; i++) {
            long long dp_new = ((long long)(i - 1) * (dp1 + dp2)) % MOD;
            dp0 = dp1;
            dp1 = dp2;
            dp2 = dp_new;
        }
        return dp2;
    }
    
    // Lucas' theorem for computing C(n, k) mod p where p is prime
    long long lucasC(long long n, long long k, long long p) {
        if (k > n) return 0;
        if (k == 0 || k == n) return 1;
        
        // Compute C(n mod p, k mod p) * C(n/p, k/p) mod p
        long long n_mod = n % p, k_mod = k % p;
        
        // Base case: compute C(n_mod, k_mod) directly
        if (k_mod > n_mod) return 0;
        
        long long result = 1;
        for (int i = 0; i < k_mod; i++) {
            result = (result * (n_mod - i)) % p;
            result = (result * modinv(i + 1, p)) % p;
        }
        
        return (result * lucasC(n / p, k / p, p)) % p;
    }
    
    // Stars and bars: number of ways to put n identical objects into k bins
    long long starsAndBars(int n, int k) {
        return C(n + k - 1, k - 1);
    }
    
    // Inclusion-exclusion principle helper
    // Count elements satisfying at least one of the conditions
    long long inclusionExclusion(vector<long long>& sets) {
        int n = sets.size();
        long long result = 0;
        
        for (int mask = 1; mask < (1 << n); mask++) {
            long long intersection = LLONG_MAX;
            int bits = __builtin_popcount(mask);
            
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    intersection = min(intersection, sets[i]);
                }
            }
            
            if (bits % 2 == 1) {
                result += intersection;
            } else {
                result -= intersection;
            }
        }
        
        return result;
    }
    
    // Multinomial coefficient: n! / (k1! * k2! * ... * km!)
    long long multinomial(vector<int>& k) {
        if (!initialized) init();
        
        int n = 0;
        for (int ki : k) n += ki;
        
        long long result = fact[n];
        for (int ki : k) {
            result = (result * inv_fact[ki]) % MOD;
        }
        
        return result;
    }
}

// Example usage:
/*
int main() {
    using namespace Combinatorics;
    
    init(); // Initialize factorials
    
    cout << "C(10, 3) = " << C(10, 3) << endl;
    cout << "P(10, 3) = " << P(10, 3) << endl;
    cout << "Catalan(4) = " << catalan(4) << endl;
    cout << "Stirling2(5, 3) = " << stirling2(5, 3) << endl;
    cout << "Derangement(4) = " << derangement(4) << endl;
    cout << "Stars and bars (5 objects, 3 bins) = " << starsAndBars(5, 3) << endl;
    
    vector<int> k = {2, 3, 1};
    cout << "Multinomial(2,3,1) = " << multinomial(k) << endl;
    
    auto bell = bellNumbers(5);
    cout << "Bell numbers up to 5: ";
    for (int i = 0; i <= 5; i++) {
        cout << bell[i] << " ";
    }
    cout << endl;
    
    return 0;
}
*/