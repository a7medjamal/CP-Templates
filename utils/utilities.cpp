#include <bits/stdc++.h>
using namespace std;

// Core utilities and macros for competitive programming
// For more specialized utilities, see:
// - io_utilities.cpp: Input/Output utilities
// - math_utilities.cpp: Mathematical utilities
// - general_utilities.cpp: General purpose utilities

namespace Utils {
    // Quick access to commonly used functions
    void fastIO() {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }
    
    #ifdef LOCAL
    #define dbg(x) cerr << #x << " = " << x << endl
    #else
    #define dbg(x)
    #endif
}

// Quick utility functions (for more comprehensive utilities, see specialized files)
template<typename T>
T gcd(T a, T b) { return b == 0 ? a : gcd(b, a % b); }

template<typename T>
T lcm(T a, T b) { return a / gcd(a, b) * b; }

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

// Commonly used macros
#define ll long long
#define ull unsigned long long
#define pii pair<int, int>
#define pll pair<ll, ll>
#define vi vector<int>
#define vll vector<ll>
#define vpii vector<pii>
#define vs vector<string>

#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) ((int)(x).size())

#define rep(i, a, b) for(int i = (a); i < (b); i++)
#define per(i, a, b) for(int i = (b) - 1; i >= (a); i--)
#define trav(a, x) for(auto& a : x)

#define yes cout << "YES\n"
#define no cout << "NO\n"
#define endl '\n'

const int INF = 1e9;
const ll LINF = 1e18;
const double EPS = 1e-9;
const double PI = acos(-1.0);
const int MOD = 1e9 + 7;

// Example usage:
/*
int main() {
    Utils::fastIO();
    
    int a = 12, b = 18;
    cout << "GCD: " << gcd(a, b) << endl;
    cout << "LCM: " << lcm(a, b) << endl;
    cout << "2^10 mod 1000000007: " << power(2LL, 10LL) << endl;
    
    dbg(a);
    dbg(b);
    
    return 0;
}
*/