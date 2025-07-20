#include <bits/stdc++.h>
using namespace std;

// Fast Fourier Transform and Number Theoretic Transform
namespace TransformAlgorithms {
    
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
        
        // Convolution using FFT
        vector<double> convolution(const vector<double>& a, const vector<double>& b) {
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
            
            vector<double> result(n);
            for (int i = 0; i < n; i++) {
                result[i] = fa[i].real();
            }
            
            while (result.size() > 1 && abs(result.back()) < 1e-9) {
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
        
        // Polynomial operations
        vector<int> add(const vector<int>& a, const vector<int>& b) {
            vector<int> result(max(a.size(), b.size()));
            for (int i = 0; i < (int)result.size(); i++) {
                if (i < a.size()) result[i] = (result[i] + a[i]) % MOD;
                if (i < b.size()) result[i] = (result[i] + b[i]) % MOD;
            }
            return result;
        }
        
        vector<int> subtract(const vector<int>& a, const vector<int>& b) {
            vector<int> result(max(a.size(), b.size()));
            for (int i = 0; i < result.size(); i++) {
                if (i < a.size()) result[i] = (result[i] + a[i]) % MOD;
                if (i < b.size()) result[i] = (result[i] - b[i] + MOD) % MOD;
            }
            return result;
        }
        
        // Polynomial inverse (for polynomials modulo x^n)
        vector<int> inverse(vector<int> a, int n) {
            vector<int> b = {(int)power(a[0], MOD - 2, MOD)};
            while (b.size() < n) {
                vector<int> a_part(a.begin(), a.begin() + min((int)a.size(), 2 * (int)b.size()));
                vector<int> prod = multiply(a_part, b);
                prod.resize(2 * b.size());
                
                vector<int> two_b = add(b, b);
                two_b.resize(2 * b.size());
                
                b = subtract(two_b, multiply(prod, b));
                b.resize(min(n, 2 * (int)b.size()));
            }
            b.resize(n);
            return b;
        }
    }
    
    // Walsh-Hadamard Transform
    namespace WHT {
        void wht(vector<int>& a, bool invert) {
            int n = a.size();
            for (int len = 1; len < n; len <<= 1) {
                for (int i = 0; i < n; i += len << 1) {
                    for (int j = 0; j < len; j++) {
                        int u = a[i + j], v = a[i + j + len];
                        a[i + j] = u + v;
                        a[i + j + len] = u - v;
                    }
                }
            }
            
            if (invert) {
                for (int& x : a) {
                    x /= n;
                }
            }
        }
        
        vector<int> xorConvolution(vector<int> a, vector<int> b) {
            int n = 1;
            while (n < max(a.size(), b.size())) {
                n <<= 1;
            }
            a.resize(n);
            b.resize(n);
            
            wht(a, false);
            wht(b, false);
            for (int i = 0; i < n; i++) {
                a[i] *= b[i];
            }
            wht(a, true);
            
            return a;
        }
    }
}

// Example usage:
/*
int main() {
    using namespace TransformAlgorithms;
    
    // FFT multiplication example
    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5, 6};
    auto result = FFT::multiply(a, b);
    
    cout << "FFT multiplication result: ";
    for (long long x : result) {
        cout << x << " ";
    }
    cout << endl;
    
    // NTT multiplication example
    auto ntt_result = NTT::multiply(a, b);
    cout << "NTT multiplication result: ";
    for (int x : ntt_result) {
        cout << x << " ";
    }
    cout << endl;
    
    return 0;
}
*/