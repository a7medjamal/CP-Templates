#include <bits/stdc++.h>
using namespace std;

// Advanced Mathematical Algorithms for ICPC
namespace AdvancedMath {
    
    // Fast Fourier Transform (FFT)
    namespace FFT {
        using cd = complex<double>;
        const double PI = acos(-1);
        
        void fft(vector<cd>& a, bool invert) {
            int n = a.size();
            
            for (int i = 1, j = 0; i < n; i++) {
                int bit = n >> 1;
                for (; j & bit; bit >>= 1) {
                    j ^= bit;
                }
                j ^= bit;
                
                if (i < j) {
                    swap(a[i], a[j]);
                }
            }
            
            for (int len = 2; len <= n; len <<= 1) {
                double ang = 2 * PI / len * (invert ? -1 : 1);
                cd wlen(cos(ang), sin(ang));
                
                for (int i = 0; i < n; i += len) {
                    cd w(1);
                    for (int j = 0; j < len / 2; j++) {
                        cd u = a[i + j], v = a[i + j + len / 2] * w;
                        a[i + j] = u + v;
                        a[i + j + len / 2] = u - v;
                        w *= wlen;
                    }
                }
            }
            
            if (invert) {
                for (cd& x : a) {
                    x /= n;
                }
            }
        }
        
        vector<long long> multiply(vector<int> const& a, vector<int> const& b) {
            vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
            int n = 1;
            while (n < a.size() + b.size()) {
                n <<= 1;
            }
            fa.resize(n);
            fb.resize(n);
            
            fft(fa, false);
            fft(fb, false);
            for (int i = 0; i < n; i++) {
                fa[i] *= fb[i];
            }
            fft(fa, true);
            
            vector<long long> result(n);
            for (int i = 0; i < n; i++) {
                result[i] = round(fa[i].real());
            }
            
            // Remove leading zeros
            while (result.size() > 1 && result.back() == 0) {
                result.pop_back();
            }
            
            return result;
        }
    }
    
    // Number Theoretic Transform (NTT)
    namespace NTT {
        const int MOD = 998244353;
        const int ROOT = 3; // primitive root of MOD
        
        long long power(long long a, long long b, long long mod) {
            long long result = 1;
            while (b > 0) {
                if (b & 1) result = (result * a) % mod;
                a = (a * a) % mod;
                b >>= 1;
            }
            return result;
        }
        
        void ntt(vector<int>& a, bool invert) {
            int n = a.size();
            
            for (int i = 1, j = 0; i < n; i++) {
                int bit = n >> 1;
                for (; j & bit; bit >>= 1) {
                    j ^= bit;
                }
                j ^= bit;
                
                if (i < j) {
                    swap(a[i], a[j]);
                }
            }
            
            for (int len = 2; len <= n; len <<= 1) {
                int wlen = invert ? power(ROOT, MOD - 1 - (MOD - 1) / len, MOD) 
                                 : power(ROOT, (MOD - 1) / len, MOD);
                
                for (int i = 0; i < n; i += len) {
                    int w = 1;
                    for (int j = 0; j < len / 2; j++) {
                        int u = a[i + j], v = (1LL * a[i + j + len / 2] * w) % MOD;
                        a[i + j] = (u + v) % MOD;
                        a[i + j + len / 2] = (u - v + MOD) % MOD;
                        w = (1LL * w * wlen) % MOD;
                    }
                }
            }
            
            if (invert) {
                int n_inv = power(n, MOD - 2, MOD);
                for (int& x : a) {
                    x = (1LL * x * n_inv) % MOD;
                }
            }
        }
        
        vector<int> multiply(vector<int> const& a, vector<int> const& b) {
            vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
            int n = 1;
            while (n < a.size() + b.size()) {
                n <<= 1;
            }
            fa.resize(n);
            fb.resize(n);
            
            ntt(fa, false);
            ntt(fb, false);
            for (int i = 0; i < n; i++) {
                fa[i] = (1LL * fa[i] * fb[i]) % MOD;
            }
            ntt(fa, true);
            
            // Remove leading zeros
            while (fa.size() > 1 && fa.back() == 0) {
                fa.pop_back();
            }
            
            return fa;
        }
    }
    
    // Matrix Operations
    class Matrix {
    public:
        vector<vector<long long>> mat;
        int n, m;
        static const long long MOD = 1e9 + 7;
        
        Matrix(int n, int m) : n(n), m(m) {
            mat.assign(n, vector<long long>(m, 0));
        }
        
        Matrix(vector<vector<long long>>& a) {
            n = a.size();
            m = a[0].size();
            mat = a;
        }
        
        Matrix operator*(const Matrix& other) const {
            Matrix result(n, other.m);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < other.m; j++) {
                    for (int k = 0; k < m; k++) {
                        result.mat[i][j] = (result.mat[i][j] + mat[i][k] * other.mat[k][j]) % MOD;
                    }
                }
            }
            return result;
        }
        
        Matrix power(long long p) const {
            Matrix result(n, n);
            for (int i = 0; i < n; i++) {
                result.mat[i][i] = 1; // Identity matrix
            }
            
            Matrix base = *this;
            while (p > 0) {
                if (p & 1) result = result * base;
                base = base * base;
                p >>= 1;
            }
            
            return result;
        }
        
        long long determinant() const {
            vector<vector<long long>> a = mat;
            long long det = 1;
            
            for (int i = 0; i < n; i++) {
                int pivot = i;
                for (int j = i + 1; j < n; j++) {
                    if (abs(a[j][i]) > abs(a[pivot][i])) {
                        pivot = j;
                    }
                }
                
                if (pivot != i) {
                    swap(a[i], a[pivot]);
                    det = (-det + MOD) % MOD;
                }
                
                if (a[i][i] == 0) return 0;
                
                det = (det * a[i][i]) % MOD;
                long long inv = power(a[i][i], MOD - 2, MOD);
                
                for (int j = i + 1; j < n; j++) {
                    long long factor = (a[j][i] * inv) % MOD;
                    for (int k = i; k < n; k++) {
                        a[j][k] = (a[j][k] - factor * a[i][k] % MOD + MOD) % MOD;
                    }
                }
            }
            
            return det;
        }
        
    private:
        long long power(long long a, long long b, long long mod) const {
            long long result = 1;
            while (b > 0) {
                if (b & 1) result = (result * a) % mod;
                a = (a * a) % mod;
                b >>= 1;
            }
            return result;
        }
    };
    
    // Linear Algebra - Gaussian Elimination
    class LinearAlgebra {
    public:
        static const double EPS = 1e-9;
        
        static int gaussianElimination(vector<vector<double>>& a, vector<double>& ans) {
            int n = a.size();
            int m = a[0].size() - 1;
            
            vector<int> where(m, -1);
            for (int col = 0, row = 0; col < m && row < n; col++) {
                int sel = row;
                for (int i = row; i < n; i++) {
                    if (abs(a[i][col]) > abs(a[sel][col])) {
                        sel = i;
                    }
                }
                
                if (abs(a[sel][col]) < EPS) continue;
                
                if (sel != row) {
                    swap(a[sel], a[row]);
                }
                
                where[col] = row;
                
                for (int i = 0; i < n; i++) {
                    if (i != row) {
                        double c = a[i][col] / a[row][col];
                        for (int j = col; j <= m; j++) {
                            a[i][j] -= a[row][j] * c;
                        }
                    }
                }
                row++;
            }
            
            ans.assign(m, 0);
            for (int i = 0; i < m; i++) {
                if (where[i] != -1) {
                    ans[i] = a[where[i]][m] / a[where[i]][i];
                }
            }
            
            for (int i = 0; i < n; i++) {
                double sum = 0;
                for (int j = 0; j < m; j++) {
                    sum += ans[j] * a[i][j];
                }
                if (abs(sum - a[i][m]) > EPS) {
                    return 0; // No solution
                }
            }
            
            for (int i = 0; i < m; i++) {
                if (where[i] == -1) {
                    return 2; // Infinite solutions
                }
            }
            
            return 1; // Unique solution
        }
    };
    
    // Discrete Logarithm (Baby-step Giant-step)
    class DiscreteLog {
    public:
        static long long power(long long a, long long b, long long mod) {
            long long result = 1;
            while (b > 0) {
                if (b & 1) result = (result * a) % mod;
                a = (a * a) % mod;
                b >>= 1;
            }
            return result;
        }
        
        static long long discreteLog(long long a, long long b, long long mod) {
            if (b == 1) return 0;
            
            long long n = sqrt(mod) + 1;
            unordered_map<long long, long long> vals;
            
            // Baby steps
            long long cur = 1;
            for (long long i = 0; i < n; i++) {
                if (cur == b) return i;
                vals[cur] = i;
                cur = (cur * a) % mod;
            }
            
            // Giant steps
            long long gamma = power(a, n, mod);
            long long y = b;
            
            for (long long i = 0; i < n; i++) {
                if (vals.count(y)) {
                    long long ans = i * n + vals[y];
                    if (ans > 0) return ans;
                }
                y = (y * gamma) % mod;
            }
            
            return -1; // No solution
        }
    };
    
    // Pollard's Rho Algorithm for Integer Factorization
    class PollardRho {
    public:
        static long long gcd(long long a, long long b) {
            return b ? gcd(b, a % b) : a;
        }
        
        static long long mulmod(long long a, long long b, long long mod) {
            return (__int128)a * b % mod;
        }
        
        static long long f(long long x, long long c, long long mod) {
            return (mulmod(x, x, mod) + c) % mod;
        }
        
        static long long pollardRho(long long n) {
            if (n % 2 == 0) return 2;
            
            long long x = 2, y = 2, c = 1, d = 1;
            
            while (d == 1) {
                x = f(x, c, n);
                y = f(f(y, c, n), c, n);
                d = gcd(abs(x - y), n);
                
                if (d == n) {
                    c++;
                    x = y = 2;
                    d = 1;
                }
            }
            
            return d;
        }
        
        static vector<long long> factorize(long long n) {
            vector<long long> factors;
            
            function<void(long long)> factor = [&](long long num) {
                if (num == 1) return;
                
                if (isPrime(num)) {
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
        static bool isPrime(long long n) {
            if (n < 2) return false;
            if (n == 2 || n == 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;
            
            for (long long i = 5; i * i <= n; i += 6) {
                if (n % i == 0 || n % (i + 2) == 0) {
                    return false;
                }
            }
            
            return true;
        }
    };
    
    // Miller-Rabin Primality Test
    class MillerRabin {
    public:
        static long long mulmod(long long a, long long b, long long mod) {
            return (__int128)a * b % mod;
        }
        
        static long long power(long long a, long long b, long long mod) {
            long long result = 1;
            while (b > 0) {
                if (b & 1) result = mulmod(result, a, mod);
                a = mulmod(a, a, mod);
                b >>= 1;
            }
            return result;
        }
        
        static bool millerRabin(long long n, long long a) {
            if (n <= 1 || a <= 1 || a >= n) return false;
            if (n == 2) return true;
            if (n % 2 == 0) return false;
            
            long long d = n - 1;
            int r = 0;
            while (d % 2 == 0) {
                d /= 2;
                r++;
            }
            
            long long x = power(a, d, n);
            if (x == 1 || x == n - 1) return true;
            
            for (int i = 0; i < r - 1; i++) {
                x = mulmod(x, x, n);
                if (x == n - 1) return true;
            }
            
            return false;
        }
        
        static bool isPrime(long long n) {
            if (n <= 1) return false;
            if (n <= 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;
            
            vector<long long> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
            
            for (long long a : witnesses) {
                if (a >= n) break;
                if (!millerRabin(n, a)) return false;
            }
            
            return true;
        }
    };
    
    // Mobius Function and Multiplicative Functions
    class MultiplicativeFunctions {
    public:
        static vector<int> mobiusFunction(int n) {
            vector<int> mu(n + 1, 1);
            vector<bool> isPrime(n + 1, true);
            
            for (int i = 2; i <= n; i++) {
                if (isPrime[i]) {
                    for (int j = i; j <= n; j += i) {
                        isPrime[j] = false;
                        mu[j] *= -1;
                    }
                    
                    for (long long j = (long long)i * i; j <= n; j += (long long)i * i) {
                        mu[j] = 0;
                    }
                }
            }
            
            return mu;
        }
        
        static vector<long long> divisorSum(int n, int k) {
            vector<long long> sigma(n + 1, 0);
            
            for (int i = 1; i <= n; i++) {
                long long pk = 1;
                for (int j = 0; j < k; j++) pk *= i;
                
                for (int j = i; j <= n; j += i) {
                    sigma[j] += pk;
                }
            }
            
            return sigma;
        }
    };
}

// Example usage:
/*
int main() {
    using namespace AdvancedMath;
    
    // FFT multiplication example
    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5, 6};
    auto result = FFT::multiply(a, b);
    
    cout << "FFT result: ";
    for (long long x : result) {
        cout << x << " ";
    }
    cout << endl;
    
    // Matrix exponentiation example
    vector<vector<long long>> mat = {{1, 1}, {1, 0}};
    Matrix fib(mat);
    Matrix result_mat = fib.power(10);
    cout << "10th Fibonacci number: " << result_mat.mat[0][1] << endl;
    
    // Primality test example
    cout << "Is 1000000007 prime? " << MillerRabin::isPrime(1000000007) << endl;
    
    return 0;
}
*/