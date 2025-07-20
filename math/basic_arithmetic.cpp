#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

// Basic arithmetic operations
namespace BasicArithmetic {
    
    // Greatest Common Divisor
    long long gcd(long long a, long long b) {
        return b == 0 ? a : gcd(b, a % b);
    }
    
    // Iterative GCD (more efficient for large numbers)
    long long gcd_iterative(long long a, long long b) {
        while (b != 0) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    
    // Least Common Multiple
    long long lcm(long long a, long long b) {
        return a / gcd(a, b) * b;
    }
    
    // Extended Euclidean Algorithm
    long long extgcd(long long a, long long b, long long& x, long long& y) {
        if (b == 0) {
            x = 1; y = 0;
            return a;
        }
        long long x1, y1;
        long long g = extgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }
    
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
    
    // Modular inverse using extended Euclidean algorithm
    long long modinv(long long a, long long m = MOD) {
        long long x, y;
        long long g = extgcd(a, m, x, y);
        if (g != 1) return -1; // No inverse exists
        return (x % m + m) % m;
    }
    
    // Modular inverse using Fermat's little theorem (when mod is prime)
    long long modinv_fermat(long long a, long long mod = MOD) {
        return power(a, mod - 2, mod);
    }
    
    // Modular multiplication (prevents overflow)
    long long mulmod(long long a, long long b, long long mod) {
        return ((a % mod) * (b % mod)) % mod;
    }
    
    // Modular addition
    long long addmod(long long a, long long b, long long mod = MOD) {
        return ((a % mod) + (b % mod)) % mod;
    }
    
    // Modular subtraction
    long long submod(long long a, long long b, long long mod = MOD) {
        return ((a % mod) - (b % mod) + mod) % mod;
    }
    
    // Binary exponentiation (without modulo)
    long long binpow(long long base, long long exp) {
        long long result = 1;
        while (exp > 0) {
            if (exp & 1) result *= base;
            base *= base;
            exp >>= 1;
        }
        return result;
    }
    
    // Matrix exponentiation for Fibonacci-like sequences
    vector<vector<long long>> matmul(const vector<vector<long long>>& A, 
                                     const vector<vector<long long>>& B, 
                                     long long mod = MOD) {
        int n = A.size(), m = B[0].size(), p = B.size();
        vector<vector<long long>> C(n, vector<long long>(m, 0));
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < p; k++) {
                    C[i][j] = addmod(C[i][j], mulmod(A[i][k], B[k][j], mod), mod);
                }
            }
        }
        return C;
    }
    
    vector<vector<long long>> matpow(vector<vector<long long>> mat, long long exp, long long mod = MOD) {
        int n = mat.size();
        vector<vector<long long>> result(n, vector<long long>(n, 0));
        
        // Initialize identity matrix
        for (int i = 0; i < n; i++) {
            result[i][i] = 1;
        }
        
        while (exp > 0) {
            if (exp & 1) {
                result = matmul(result, mat, mod);
            }
            mat = matmul(mat, mat, mod);
            exp >>= 1;
        }
        return result;
    }
    
    // Nth Fibonacci number using matrix exponentiation
    long long fibonacci(long long n, long long mod = MOD) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        
        vector<vector<long long>> fib_mat = {{1, 1}, {1, 0}};
        vector<vector<long long>> result = matpow(fib_mat, n - 1, mod);
        
        return result[0][0];
    }
    
    // Check if a number is perfect square
    bool isPerfectSquare(long long n) {
        if (n < 0) return false;
        long long root = sqrt(n);
        return root * root == n;
    }
    
    // Integer square root
    long long isqrt(long long n) {
        if (n < 0) return -1;
        long long x = sqrt(n);
        while (x * x > n) x--;
        while ((x + 1) * (x + 1) <= n) x++;
        return x;
    }
    
    // Check if a number is perfect power
    bool isPerfectPower(long long n, int k) {
        if (n <= 1) return n == 1;
        long long root = pow(n, 1.0 / k);
        
        // Check root and root±1 due to floating point precision
        for (long long r = max(1LL, root - 1); r <= root + 1; r++) {
            if (binpow(r, k) == n) return true;
        }
        return false;
    }
    
    // Catalan numbers
    long long catalan(int n, long long mod = MOD) {
        if (n <= 1) return 1;
        
        // C(n) = (2n)! / ((n+1)! * n!)
        // C(n) = C(2n, n) / (n+1)
        long long result = 1;
        for (int i = 0; i < n; i++) {
            result = mulmod(result, 2 * n - i, mod);
            result = mulmod(result, modinv_fermat(i + 1, mod), mod);
        }
        result = mulmod(result, modinv_fermat(n + 1, mod), mod);
        return result;
    }
    
    // Binomial coefficient C(n, k)
    long long binomial(int n, int k, long long mod = MOD) {
        if (k > n || k < 0) return 0;
        if (k == 0 || k == n) return 1;
        
        k = min(k, n - k); // Take advantage of symmetry
        
        long long result = 1;
        for (int i = 0; i < k; i++) {
            result = mulmod(result, n - i, mod);
            result = mulmod(result, modinv_fermat(i + 1, mod), mod);
        }
        return result;
    }
    
    // Stirling numbers of the second kind S(n, k)
    long long stirling2(int n, int k, long long mod = MOD) {
        if (n == 0 && k == 0) return 1;
        if (n == 0 || k == 0) return 0;
        if (k > n) return 0;
        
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, 0));
        dp[0][0] = 1;
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= min(i, k); j++) {
                dp[i][j] = addmod(mulmod(j, dp[i-1][j], mod), dp[i-1][j-1], mod);
            }
        }
        
        return dp[n][k];
    }
}

// Example usage:
/*
int main() {
    using namespace BasicArithmetic;
    
    cout << "GCD(48, 18) = " << gcd(48, 18) << endl;
    cout << "LCM(48, 18) = " << lcm(48, 18) << endl;
    cout << "2^10 mod 1000000007 = " << power(2, 10) << endl;
    cout << "Inverse of 3 mod 1000000007 = " << modinv(3) << endl;
    cout << "10th Fibonacci number = " << fibonacci(10) << endl;
    cout << "C(10, 3) = " << binomial(10, 3) << endl;
    cout << "5th Catalan number = " << catalan(5) << endl;
    
    return 0;
}
*/