#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

// Number theory functions
namespace NumberTheoryFunctions {
    
    // Fast modular exponentiation (helper function)
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
    
    // Extended GCD (helper function)
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
    
    // Modular inverse
    long long modinv(long long a, long long m = MOD) {
        long long x, y;
        long long g = extgcd(a, m, x, y);
        if (g != 1) return -1;
        return (x % m + m) % m;
    }
    
    // Euler's totient function φ(n)
    long long phi(long long n) {
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
    
    // Precomputed phi values using sieve
    vector<long long> phiSieve(int n) {
        vector<long long> phi(n + 1);
        for (int i = 0; i <= n; i++) phi[i] = i;
        
        for (int i = 2; i <= n; i++) {
            if (phi[i] == i) { // i is prime
                for (int j = i; j <= n; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }
        return phi;
    }
    
    // Sum of totient function from 1 to n
    long long sumPhi(long long n) {
        // Sum of φ(i) for i = 1 to n
        // This is approximately 3n²/π²
        long long sum = 0;
        vector<long long> phi_values = phiSieve(n);
        for (int i = 1; i <= n; i++) {
            sum += phi_values[i];
        }
        return sum;
    }
    
    // Number of divisors τ(n)
    long long countDivisors(long long n) {
        long long count = 1;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                int exp = 0;
                while (n % i == 0) {
                    n /= i;
                    exp++;
                }
                count *= (exp + 1);
            }
        }
        if (n > 1) count *= 2;
        return count;
    }
    
    // Sum of divisors σ(n)
    long long sumDivisors(long long n) {
        long long sum = 1;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                long long p = i, exp = 0;
                while (n % i == 0) {
                    n /= i;
                    exp++;
                }
                sum *= (power(p, exp + 1) - 1) / (p - 1);
            }
        }
        if (n > 1) sum *= (n + 1);
        return sum;
    }
    
    // Get all divisors of n
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
    
    // Sum of divisors function σ_k(n) where k is the power
    long long sumDivisorsPower(long long n, int k) {
        long long sum = 1;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                long long p = i, exp = 0;
                while (n % i == 0) {
                    n /= i;
                    exp++;
                }
                
                if (k == 0) {
                    sum *= (exp + 1);
                } else {
                    long long pk = power(p, k);
                    sum *= (power(pk, exp + 1) - 1) / (pk - 1);
                }
            }
        }
        if (n > 1) {
            if (k == 0) {
                sum *= 2;
            } else {
                sum *= (power(n, k) + 1);
            }
        }
        return sum;
    }
    
    // Chinese Remainder Theorem
    pair<long long, long long> crt(vector<long long>& a, vector<long long>& m) {
        long long M = 1;
        for (long long mod : m) M *= mod;
        
        long long result = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            long long Mi = M / m[i];
            long long yi = modinv(Mi, m[i]);
            if (yi == -1) return {-1, -1}; // No solution
            result = (result + a[i] * Mi % M * yi % M) % M;
        }
        
        return {result, M};
    }
    
    // Extended Chinese Remainder Theorem (handles non-coprime moduli)
    pair<long long, long long> extendedCRT(vector<long long>& a, vector<long long>& m) {
        long long x = a[0], lcm = m[0];
        
        for (int i = 1; i < (int)a.size(); i++) {
            long long a1 = x, m1 = lcm;
            long long a2 = a[i], m2 = m[i];
            
            long long g = __gcd(m1, m2);
            if ((a2 - a1) % g != 0) {
                return {-1, -1}; // No solution
            }
            
            // Solve: x ≡ a1 (mod m1) and x ≡ a2 (mod m2)
            long long p, q;
            extgcd(m1 / g, m2 / g, p, q);
            
            long long mod = m1 / g * m2;
            x = ((a1 + m1 * ((a2 - a1) / g * p % (m2 / g))) % mod + mod) % mod;
            lcm = mod;
        }
        
        return {x, lcm};
    }
    
    // Möbius function μ(n)
    int mobius(long long n) {
        int factors = 0;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                if (n % (i * i) == 0) return 0; // Square factor
                n /= i;
                factors++;
            }
        }
        if (n > 1) factors++;
        return (factors % 2 == 0) ? 1 : -1;
    }
    
    // Precomputed Möbius function using sieve
    vector<int> mobiusSieve(int n) {
        vector<int> mu(n + 1, 1);
        vector<bool> is_prime(n + 1, true);
        
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) {
                for (int j = i; j <= n; j += i) {
                    is_prime[j] = false;
                    mu[j] *= -1;
                }
                for (long long j = (long long)i * i; j <= n; j += (long long)i * i) {
                    mu[j] = 0;
                }
            }
        }
        
        return mu;
    }
    
    // Carmichael function λ(n)
    long long carmichael(long long n) {
        if (n == 1) return 1;
        
        long long result = 1;
        
        // Handle power of 2
        if (n % 2 == 0) {
            int exp = 0;
            while (n % 2 == 0) {
                n /= 2;
                exp++;
            }
            if (exp == 1) result = 1;
            else if (exp == 2) result = 2;
            else result = power(2, exp - 2);
        }
        
        // Handle odd prime powers
        for (long long i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                long long p = i, exp = 0;
                while (n % i == 0) {
                    n /= i;
                    exp++;
                }
                long long lambda_p = power(p, exp - 1) * (p - 1);
                result = result / __gcd(result, lambda_p) * lambda_p;
            }
        }
        
        if (n > 1) {
            long long lambda_p = n - 1;
            result = result / __gcd(result, lambda_p) * lambda_p;
        }
        
        return result;
    }
    
    // Jordan's totient function J_k(n)
    long long jordanTotient(long long n, int k) {
        long long result = power(n, k);
        
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0) n /= i;
                result -= result / power(i, k);
            }
        }
        
        if (n > 1) {
            result -= result / power(n, k);
        }
        
        return result;
    }
    
    // Legendre symbol (a/p) where p is an odd prime
    int legendre(long long a, long long p) {
        if (a % p == 0) return 0;
        long long result = power(a, (p - 1) / 2, p);
        return result == 1 ? 1 : -1;
    }
    
    // Jacobi symbol (a/n) - generalization of Legendre symbol
    int jacobi(long long a, long long n) {
        if (__gcd(a, n) != 1) return 0;
        
        int result = 1;
        a %= n;
        
        while (a != 0) {
            while (a % 2 == 0) {
                a /= 2;
                if (n % 8 == 3 || n % 8 == 5) result = -result;
            }
            swap(a, n);
            if (a % 4 == 3 && n % 4 == 3) result = -result;
            a %= n;
        }
        
        return n == 1 ? result : 0;
    }
    
    // Check if a is a quadratic residue modulo p
    bool isQuadraticResidue(long long a, long long p) {
        return legendre(a, p) == 1;
    }
    
    // Tonelli-Shanks algorithm for modular square root
    long long modularSqrt(long long a, long long p) {
        if (legendre(a, p) != 1) return -1; // No solution
        
        if (p % 4 == 3) {
            return power(a, (p + 1) / 4, p);
        }
        
        // Tonelli-Shanks algorithm
        long long Q = p - 1, S = 0;
        while (Q % 2 == 0) {
            Q /= 2;
            S++;
        }
        
        if (S == 1) {
            return power(a, (p + 1) / 4, p);
        }
        
        // Find quadratic non-residue
        long long z = 2;
        while (legendre(z, p) != -1) z++;
        
        long long M = S, c = power(z, Q, p), t = power(a, Q, p), R = power(a, (Q + 1) / 2, p);
        
        while (t != 1) {
            long long i = 1, temp = (t * t) % p;
            while (temp != 1) {
                temp = (temp * temp) % p;
                i++;
            }
            
            long long b = power(c, power(2, M - i - 1), p);
            M = i;
            c = (b * b) % p;
            t = (t * c) % p;
            R = (R * b) % p;
        }
        
        return R;
    }
}

// Example usage:
/*
int main() {
    using namespace NumberTheoryFunctions;
    
    cout << "φ(12) = " << phi(12) << endl;
    cout << "τ(12) = " << countDivisors(12) << endl;
    cout << "σ(12) = " << sumDivisors(12) << endl;
    cout << "μ(12) = " << mobius(12) << endl;
    cout << "λ(12) = " << carmichael(12) << endl;
    
    auto divisors = getDivisors(12);
    cout << "Divisors of 12: ";
    for (long long d : divisors) {
        cout << d << " ";
    }
    cout << endl;
    
    vector<long long> a = {2, 3, 2};
    vector<long long> m = {3, 5, 7};
    auto [x, mod] = crt(a, m);
    cout << "CRT solution: x ≡ " << x << " (mod " << mod << ")" << endl;
    
    cout << "Legendre(3, 7) = " << legendre(3, 7) << endl;
    cout << "Jacobi(3, 9) = " << jacobi(3, 9) << endl;
    
    return 0;
}
*/