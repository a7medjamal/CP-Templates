#include <bits/stdc++.h>
using namespace std;

// Type definitions
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<pii> vpii;
typedef vector<string> vs;

// Macros
#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)(x).size())
#define rep(i, a, b) for(int i = (a); i < (b); i++)
#define per(i, a, b) for(int i = (b) - 1; i >= (a); i--)
#define trav(a, x) for(auto& a : x)

// Constants
const int MOD = 1e9 + 7;
const int INF = 1e9;
const ll LINF = 1e18;
const double EPS = 1e-9;
const double PI = acos(-1.0);

// Utility functions
template<typename T> void print_vector(const vector<T>& v) {
    for(int i = 0; i < sz(v); i++) {
        cout << v[i] << (i == sz(v) - 1 ? "\n" : " ");
    }
}

template<typename T> T gcd(T a, T b) {
    return b == 0 ? a : gcd(b, a % b);
}

template<typename T> T lcm(T a, T b) {
    return a / gcd(a, b) * b;
}

template<typename T> T power(T base, T exp, T mod = MOD) {
    T result = 1;
    while(exp > 0) {
        if(exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

void solve() {
    // Your solution here
    
}

int main() {
    fast_io;
    
    int t = 1;
    // cin >> t;  // Uncomment for multiple test cases
    
    while(t--) {
        solve();
    }
    
    return 0;
}