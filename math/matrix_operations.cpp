#include <bits/stdc++.h>
using namespace std;

// Matrix Operations and Linear Algebra
namespace MatrixOperations {
    
    // Basic Matrix class
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
        
        Matrix operator+(const Matrix& other) const {
            Matrix result(n, m);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result.mat[i][j] = (mat[i][j] + other.mat[i][j]) % MOD;
                }
            }
            return result;
        }
        
        Matrix operator-(const Matrix& other) const {
            Matrix result(n, m);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result.mat[i][j] = (mat[i][j] - other.mat[i][j] + MOD) % MOD;
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
        
        // Determinant calculation
        long long determinant() const {
            if (n != m) return 0;
            
            vector<vector<long long>> temp = mat;
            long long det = 1;
            
            for (int i = 0; i < n; i++) {
                int pivot = i;
                for (int j = i + 1; j < n; j++) {
                    if (abs(temp[j][i]) > abs(temp[pivot][i])) {
                        pivot = j;
                    }
                }
                
                if (temp[pivot][i] == 0) {
                    return 0;
                }
                
                if (pivot != i) {
                    swap(temp[i], temp[pivot]);
                    det = (MOD - det) % MOD;
                }
                
                det = (det * temp[i][i]) % MOD;
                
                long long inv = modInverse(temp[i][i]);
                for (int j = i + 1; j < n; j++) {
                    long long factor = (temp[j][i] * inv) % MOD;
                    for (int k = i; k < n; k++) {
                        temp[j][k] = (temp[j][k] - factor * temp[i][k] % MOD + MOD) % MOD;
                    }
                }
            }
            
            return det;
        }
        
        // Matrix inverse
        Matrix inverse() const {
            if (n != m) throw invalid_argument("Matrix must be square");
            
            vector<vector<long long>> aug(n, vector<long long>(2 * n, 0));
            
            // Create augmented matrix [A|I]
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    aug[i][j] = mat[i][j];
                }
                aug[i][i + n] = 1;
            }
            
            // Gaussian elimination
            for (int i = 0; i < n; i++) {
                // Find pivot
                int pivot = i;
                for (int j = i + 1; j < n; j++) {
                    if (abs(aug[j][i]) > abs(aug[pivot][i])) {
                        pivot = j;
                    }
                }
                
                if (aug[pivot][i] == 0) {
                    throw invalid_argument("Matrix is not invertible");
                }
                
                if (pivot != i) {
                    swap(aug[i], aug[pivot]);
                }
                
                // Make diagonal element 1
                long long inv = modInverse(aug[i][i]);
                for (int j = 0; j < 2 * n; j++) {
                    aug[i][j] = (aug[i][j] * inv) % MOD;
                }
                
                // Eliminate column
                for (int j = 0; j < n; j++) {
                    if (i != j) {
                        long long factor = aug[j][i];
                        for (int k = 0; k < 2 * n; k++) {
                            aug[j][k] = (aug[j][k] - factor * aug[i][k] % MOD + MOD) % MOD;
                        }
                    }
                }
            }
            
            // Extract inverse matrix
            Matrix result(n, n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    result.mat[i][j] = aug[i][j + n];
                }
            }
            
            return result;
        }
        
        // Transpose
        Matrix transpose() const {
            Matrix result(m, n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result.mat[j][i] = mat[i][j];
                }
            }
            return result;
        }
        
        void print() const {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    cout << mat[i][j] << " ";
                }
                cout << endl;
            }
        }
        
    private:
        long long modInverse(long long a) const {
            return power(a, MOD - 2);
        }
        
        long long power(long long a, long long b) const {
            long long result = 1;
            while (b > 0) {
                if (b & 1) result = (result * a) % MOD;
                a = (a * a) % MOD;
                b >>= 1;
            }
            return result;
        }
    };
    
    // Linear Algebra operations
    namespace LinearAlgebra {
        
        // Gaussian Elimination
        pair<int, vector<double>> gaussianElimination(vector<vector<double>> a, vector<double> b) {
            int n = a.size();
            int m = a[0].size();
            
            vector<int> where(m, -1);
            for (int col = 0, row = 0; col < m && row < n; col++) {
                int sel = row;
                for (int i = row; i < n; i++) {
                    if (abs(a[i][col]) > abs(a[sel][col])) {
                        sel = i;
                    }
                }
                
                if (abs(a[sel][col]) < 1e-9) {
                    continue;
                }
                
                if (sel != row) {
                    swap(a[sel], a[row]);
                    swap(b[sel], b[row]);
                }
                
                where[col] = row;
                
                for (int i = 0; i < n; i++) {
                    if (i != row) {
                        double c = a[i][col] / a[row][col];
                        for (int j = col; j < m; j++) {
                            a[i][j] -= a[row][j] * c;
                        }
                        b[i] -= b[row] * c;
                    }
                }
                row++;
            }
            
            vector<double> ans(m, 0);
            for (int i = 0; i < m; i++) {
                if (where[i] != -1) {
                    ans[i] = b[where[i]] / a[where[i]][i];
                }
            }
            
            for (int i = 0; i < n; i++) {
                double sum = 0;
                for (int j = 0; j < m; j++) {
                    sum += ans[j] * a[i][j];
                }
                if (abs(sum - b[i]) > 1e-9) {
                    return {0, {}}; // No solution
                }
            }
            
            for (int i = 0; i < m; i++) {
                if (where[i] == -1) {
                    return {2, ans}; // Infinite solutions
                }
            }
            
            return {1, ans}; // Unique solution
        }
        
        // Matrix rank
        int matrixRank(vector<vector<double>> a) {
            int n = a.size();
            int m = a[0].size();
            
            int rank = 0;
            for (int col = 0, row = 0; col < m && row < n; col++) {
                int sel = row;
                for (int i = row; i < n; i++) {
                    if (abs(a[i][col]) > abs(a[sel][col])) {
                        sel = i;
                    }
                }
                
                if (abs(a[sel][col]) < 1e-9) {
                    continue;
                }
                
                if (sel != row) {
                    swap(a[sel], a[row]);
                }
                
                for (int i = row + 1; i < n; i++) {
                    double c = a[i][col] / a[row][col];
                    for (int j = col; j < m; j++) {
                        a[i][j] -= a[row][j] * c;
                    }
                }
                
                rank++;
                row++;
            }
            
            return rank;
        }
        
        // LU Decomposition
        pair<Matrix, Matrix> luDecomposition(const Matrix& A) {
            int n = A.n;
            Matrix L(n, n), U(n, n);
            
            for (int i = 0; i < n; i++) {
                // Upper triangular
                for (int k = i; k < n; k++) {
                    long long sum = 0;
                    for (int j = 0; j < i; j++) {
                        sum = (sum + L.mat[i][j] * U.mat[j][k]) % Matrix::MOD;
                    }
                    U.mat[i][k] = (A.mat[i][k] - sum + Matrix::MOD) % Matrix::MOD;
                }
                
                // Lower triangular
                for (int k = i; k < n; k++) {
                    if (i == k) {
                        L.mat[i][i] = 1;
                    } else {
                        long long sum = 0;
                        for (int j = 0; j < i; j++) {
                            sum = (sum + L.mat[k][j] * U.mat[j][i]) % Matrix::MOD;
                        }
                        if (U.mat[i][i] != 0) {
                            L.mat[k][i] = ((A.mat[k][i] - sum + Matrix::MOD) * modInverse(U.mat[i][i])) % Matrix::MOD;
                        }
                    }
                }
            }
            
            return {L, U};
        }
        
        // Helper functions
        long long modInverse(long long a) {
            return power(a, Matrix::MOD - 2);
        }
        
        long long power(long long a, long long b) {
            long long result = 1;
            while (b > 0) {
                if (b & 1) result = (result * a) % Matrix::MOD;
                a = (a * a) % Matrix::MOD;
                b >>= 1;
            }
            return result;
        }
    }
    
    
    // Special matrices
    namespace SpecialMatrices {
        
        // Identity matrix
        Matrix identity(int n) {
            Matrix result(n, n);
            for (int i = 0; i < n; i++) {
                result.mat[i][i] = 1;
            }
            return result;
        }
        
        // Fibonacci matrix
        Matrix fibonacciMatrix() {
            Matrix fib(2, 2);
            fib.mat[0][0] = 1; fib.mat[0][1] = 1;
            fib.mat[1][0] = 1; fib.mat[1][1] = 0;
            return fib;
        }
        
        // Calculate nth Fibonacci number using matrix exponentiation
        long long fibonacci(long long n) {
            if (n == 0) return 0;
            if (n == 1) return 1;
            
            Matrix fib = fibonacciMatrix();
            Matrix result = fib.power(n - 1);
            return result.mat[0][0];
        }
        
        // Rotation matrix (2D)
        Matrix rotationMatrix(double angle) {
            Matrix rot(2, 2);
            long long cosVal = (long long)(cos(angle) * 1000000) % Matrix::MOD;
            long long sinVal = (long long)(sin(angle) * 1000000) % Matrix::MOD;
            
            rot.mat[0][0] = cosVal; rot.mat[0][1] = -sinVal;
            rot.mat[1][0] = sinVal; rot.mat[1][1] = cosVal;
            return rot;
        }
    }
}

// Example usage:
/*
int main() {
    using namespace MatrixOperations;
    
    // Matrix multiplication example
    vector<vector<long long>> a = {{1, 2}, {3, 4}};
    vector<vector<long long>> b = {{5, 6}, {7, 8}};
    
    Matrix matA(a);
    Matrix matB(b);
    
    Matrix result = matA * matB;
    cout << "Matrix multiplication result:" << endl;
    result.print();
    
    // Fibonacci using matrix exponentiation
    cout << "10th Fibonacci number: " << SpecialMatrices::fibonacci(10) << endl;
    
    // Gaussian elimination example
    vector<vector<double>> coeffs = {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}};
    vector<double> constants = {8, -11, -3};
    
    auto [status, solution] = LinearAlgebra::gaussianElimination(coeffs, constants);
    if (status == 1) {
        cout << "Solution: ";
        for (double x : solution) {
            cout << x << " ";
        }
        cout << endl;
    }
    
    return 0;
}
*/