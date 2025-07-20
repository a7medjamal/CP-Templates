#include <bits/stdc++.h>
using namespace std;

// Discrete Mathematics Algorithms
namespace DiscreteMath {
    
    // Discrete Logarithm using Baby-step Giant-step
    namespace DiscreteLog {
        long long power(long long a, long long b, long long mod) {
            long long result = 1;
            a %= mod;
            while (b > 0) {
                if (b & 1) result = (__int128)result * a % mod;
                a = (__int128)a * a % mod;
                b >>= 1;
            }
            return result;
        }
        
        // Find x such that a^x ≡ b (mod m)
        long long babyStepGiantStep(long long a, long long b, long long m) {
            if (b == 1) return 0;
            
            long long n = sqrt(m) + 1;
            unordered_map<long long, long long> baby_steps;
            
            // Baby steps: compute a^j mod m for j = 0, 1, ..., n-1
            long long gamma = 1;
            for (long long j = 0; j < n; j++) {
                if (baby_steps.find(gamma) == baby_steps.end()) {
                    baby_steps[gamma] = j;
                }
                gamma = (__int128)gamma * a % m;
            }
            
            // Giant steps: compute b * (a^(-n))^i mod m for i = 0, 1, ..., n-1
            long long factor = power(a, m - 1 - n, m); // a^(-n) mod m
            long long y = b;
            
            for (long long i = 0; i < n; i++) {
                if (baby_steps.find(y) != baby_steps.end()) {
                    long long ans = i * n + baby_steps[y];
                    if (ans > 0) return ans;
                }
                y = (__int128)y * factor % m;
            }
            
            return -1; // No solution
        }
        
        // Pohlig-Hellman algorithm for discrete logarithm
        long long pohligHellman(long long a, long long b, long long p, vector<pair<long long, int>>& factors) {
            vector<long long> remainders;
            vector<long long> moduli;
            
            for (auto [prime, exp] : factors) {
                long long pk = 1;
                for (int i = 0; i < exp; i++) pk *= prime;
                
                long long a_reduced = power(a, (p - 1) / pk, p);
                long long b_reduced = power(b, (p - 1) / pk, p);
                
                long long x = babyStepGiantStep(a_reduced, b_reduced, p);
                if (x == -1) return -1;
                
                remainders.push_back(x);
                moduli.push_back(pk);
            }
            
            // Use Chinese Remainder Theorem to combine results
            return chineseRemainderTheorem(remainders, moduli);
        }
        
    private:
        long long chineseRemainderTheorem(vector<long long>& remainders, vector<long long>& moduli) {
            long long result = 0;
            long long product = 1;
            
            for (long long mod : moduli) {
                product *= mod;
            }
            
            for (int i = 0; i < (int)remainders.size(); i++) {
                long long partial_product = product / moduli[i];
                long long inverse = power(partial_product, moduli[i] - 2, moduli[i]);
                result = (result + remainders[i] * partial_product % product * inverse) % product;
            }
            
            return result;
        }
    }
    
    // Pollard's Rho Algorithm for Integer Factorization
    namespace PollardRho {
        long long gcd(long long a, long long b) {
            while (b) {
                a %= b;
                swap(a, b);
            }
            return a;
        }
        
        long long multiply(long long a, long long b, long long mod) {
            return (__int128)a * b % mod;
        }
        
        long long f(long long x, long long c, long long n) {
            return (multiply(x, x, n) + c) % n;
        }
        
        long long pollardRho(long long n) {
            if (n % 2 == 0) return 2;
            
            for (long long c = 1; c < n; c++) {
                long long x = 2, y = 2, d = 1;
                
                while (d == 1) {
                    x = f(x, c, n);
                    y = f(f(y, c, n), c, n);
                    d = gcd(abs(x - y), n);
                }
                
                if (d != n) return d;
            }
            
            return n;
        }
        
        vector<long long> factorize(long long n) {
            vector<long long> factors;
            
            function<void(long long)> factor = [&](long long num) {
                if (num == 1) return;
                
                if (millerRabin(num)) {
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
        
    private:
        bool millerRabin(long long n) {
            if (n < 2) return false;
            if (n == 2 || n == 3) return true;
            if (n % 2 == 0) return false;
            
            long long d = n - 1;
            int r = 0;
            while (d % 2 == 0) {
                d /= 2;
                r++;
            }
            
            vector<long long> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
            
            for (long long a : witnesses) {
                if (a >= n) continue;
                
                long long x = power(a, d, n);
                if (x == 1 || x == n - 1) continue;
                
                bool composite = true;
                for (int i = 0; i < r - 1; i++) {
                    x = multiply(x, x, n);
                    if (x == n - 1) {
                        composite = false;
                        break;
                    }
                }
                
                if (composite) return false;
            }
            
            return true;
        }
        
        long long power(long long a, long long b, long long mod) {
            long long result = 1;
            a %= mod;
            while (b > 0) {
                if (b & 1) result = multiply(result, a, mod);
                a = multiply(a, a, mod);
                b >>= 1;
            }
            return result;
        }
    }
    
    // Miller-Rabin Primality Test
    namespace MillerRabin {
        long long multiply(long long a, long long b, long long mod) {
            return (__int128)a * b % mod;
        }
        
        long long power(long long a, long long b, long long mod) {
            long long result = 1;
            a %= mod;
            while (b > 0) {
                if (b & 1) result = multiply(result, a, mod);
                a = multiply(a, a, mod);
                b >>= 1;
            }
            return result;
        }
        
        bool millerTest(long long d, long long n) {
            vector<long long> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
            
            for (long long a : witnesses) {
                if (a >= n) continue;
                
                long long x = power(a, d, n);
                if (x == 1 || x == n - 1) continue;
                
                while (d != n - 1) {
                    x = multiply(x, x, n);
                    d *= 2;
                    
                    if (x == 1) return false;
                    if (x == n - 1) break;
                }
                
                if (x != n - 1) return false;
            }
            
            return true;
        }
        
        bool isPrime(long long n) {
            if (n <= 1 || n == 4) return false;
            if (n <= 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;
            
            long long d = n - 1;
            while (d % 2 == 0) {
                d /= 2;
            }
            
            return millerTest(d, n);
        }
        
        // Generate next prime after n
        long long nextPrime(long long n) {
            if (n <= 1) return 2;
            if (n % 2 == 0) n++;
            else n += 2;
            
            while (!isPrime(n)) {
                n += 2;
            }
            
            return n;
        }
        
        // Generate all primes up to n using Miller-Rabin
        vector<long long> generatePrimes(long long n) {
            vector<long long> primes;
            if (n >= 2) primes.push_back(2);
            
            for (long long i = 3; i <= n; i += 2) {
                if (isPrime(i)) {
                    primes.push_back(i);
                }
            }
            
            return primes;
        }
    }
    
    // Multiplicative Functions
    namespace MultiplicativeFunctions {
        // Mobius function
        vector<int> mobius(int n) {
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
        
        // Euler's totient function
        vector<int> phi(int n) {
            vector<int> result(n + 1);
            iota(result.begin(), result.end(), 0);
            
            for (int i = 2; i <= n; i++) {
                if (result[i] == i) { // i is prime
                    for (int j = i; j <= n; j += i) {
                        result[j] -= result[j] / i;
                    }
                }
            }
            
            return result;
        }
        
        // Sum of divisors function
        vector<long long> sigma(int n, int k = 1) {
            vector<long long> result(n + 1, 1);
            
            for (int i = 2; i <= n; i++) {
                for (int j = i; j <= n; j += i) {
                    long long power_i = 1;
                    for (int p = 0; p < k; p++) {
                        power_i *= i;
                    }
                    result[j] += power_i;
                }
            }
            
            return result;
        }
        
        // Number of divisors function
        vector<int> tau(int n) {
            vector<int> result(n + 1, 0);
            
            for (int i = 1; i <= n; i++) {
                for (int j = i; j <= n; j += i) {
                    result[j]++;
                }
            }
            
            return result;
        }
        
        // Dirichlet convolution
        vector<long long> dirichletConvolution(const vector<long long>& f, const vector<long long>& g) {
            int n = min(f.size(), g.size()) - 1;
            vector<long long> result(n + 1, 0);
            
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j * i <= n; j++) {
                    result[i * j] += f[i] * g[j];
                }
            }
            
            return result;
        }
    }
    
    // Quadratic Residues
    namespace QuadraticResidues {
        long long power(long long a, long long b, long long mod) {
            long long result = 1;
            a %= mod;
            while (b > 0) {
                if (b & 1) result = (__int128)result * a % mod;
                a = (__int128)a * a % mod;
                b >>= 1;
            }
            return result;
        }
        
        // Legendre symbol
        int legendre(long long a, long long p) {
            if (a % p == 0) return 0;
            long long result = power(a, (p - 1) / 2, p);
            return result == 1 ? 1 : -1;
        }
        
        // Jacobi symbol
        int jacobi(long long a, long long n) {
            if (n <= 0 || n % 2 == 0) return 0;
            
            int result = 1;
            a %= n;
            
            while (a != 0) {
                while (a % 2 == 0) {
                    a /= 2;
                    if (n % 8 == 3 || n % 8 == 5) {
                        result = -result;
                    }
                }
                
                swap(a, n);
                if (a % 4 == 3 && n % 4 == 3) {
                    result = -result;
                }
                
                a %= n;
            }
            
            return n == 1 ? result : 0;
        }
        
        // Tonelli-Shanks algorithm for square root modulo prime
        long long tonelliShanks(long long n, long long p) {
            if (legendre(n, p) != 1) return -1;
            
            if (p % 4 == 3) {
                return power(n, (p + 1) / 4, p);
            }
            
            long long q = p - 1;
            int s = 0;
            while (q % 2 == 0) {
                q /= 2;
                s++;
            }
            
            long long z = 2;
            while (legendre(z, p) != -1) {
                z++;
            }
            
            long long m = s;
            long long c = power(z, q, p);
            long long t = power(n, q, p);
            long long r = power(n, (q + 1) / 2, p);
            
            while (t != 1) {
                long long temp = t;
                int i = 1;
                while (temp != 1) {
                    temp = (__int128)temp * temp % p;
                    i++;
                }
                
                long long b = power(c, 1LL << (m - i - 1), p);
                m = i;
                c = (__int128)b * b % p;
                t = (__int128)t * c % p;
                r = (__int128)r * b % p;
            }
            
            return r;
        }
    }
}

// Example usage:
/*
int main() {
    using namespace DiscreteMath;
    
    // Miller-Rabin primality test
    long long n = 1000000007;
    cout << n << " is " << (MillerRabin::isPrime(n) ? "prime" : "composite") << endl;
    
    // Pollard's Rho factorization
    long long num = 1001;
    auto factors = PollardRho::factorize(num);
    cout << "Factors of " << num << ": ";
    for (long long f : factors) {
        cout << f << " ";
    }
    cout << endl;
    
    // Discrete logarithm
    long long result = DiscreteLog::babyStepGiantStep(2, 8, 13);
    cout << "2^x ≡ 8 (mod 13), x = " << result << endl;
    
    // Mobius function
    auto mu = MultiplicativeFunctions::mobius(20);
    cout << "Mobius function values: ";
    for (int i = 1; i <= 20; i++) {
        cout << mu[i] << " ";
    }
    cout << endl;
    
    return 0;
}
*/