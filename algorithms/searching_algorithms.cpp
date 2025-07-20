#include <bits/stdc++.h>
using namespace std;

// Searching Algorithms
namespace SearchingAlgorithms {
    
    // Binary Search Variants
    
    // Standard binary search
    int binarySearch(const vector<int>& arr, int target) {
        int left = 0, right = arr.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1; // Not found
    }
    
    // Lower bound (first position where element >= target)
    int lowerBound(const vector<int>& arr, int target) {
        int left = 0, right = arr.size();
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // Upper bound (first position where element > target)
    int upperBound(const vector<int>& arr, int target) {
        int left = 0, right = arr.size();
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // Binary search on answer
    template<typename T>
    T binarySearchAnswer(T left, T right, function<bool(T)> check) {
        while (left < right) {
            T mid = left + (right - left) / 2;
            
            if (check(mid)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
    // Ternary Search
    
    // Ternary search for maximum (unimodal function)
    double ternarySearchMax(double left, double right, function<double(double)> f, double eps = 1e-9) {
        while (right - left > eps) {
            double m1 = left + (right - left) / 3;
            double m2 = right - (right - left) / 3;
            
            if (f(m1) < f(m2)) {
                left = m1;
            } else {
                right = m2;
            }
        }
        
        return (left + right) / 2;
    }
    
    // Ternary search for minimum (unimodal function)
    double ternarySearchMin(double left, double right, function<double(double)> f, double eps = 1e-9) {
        while (right - left > eps) {
            double m1 = left + (right - left) / 3;
            double m2 = right - (right - left) / 3;
            
            if (f(m1) > f(m2)) {
                left = m1;
            } else {
                right = m2;
            }
        }
        
        return (left + right) / 2;
    }
    
    // Integer ternary search
    int ternarySearchInt(int left, int right, function<long long(int)> f) {
        while (right - left > 2) {
            int m1 = left + (right - left) / 3;
            int m2 = right - (right - left) / 3;
            
            if (f(m1) > f(m2)) {
                left = m1;
            } else {
                right = m2;
            }
        }
        
        // Check remaining candidates
        int best = left;
        for (int i = left + 1; i <= right; i++) {
            if (f(i) < f(best)) {
                best = i;
            }
        }
        
        return best;
    }
    
    // Exponential Search
    int exponentialSearch(const vector<int>& arr, int target) {
        if (arr[0] == target) return 0;
        
        int i = 1;
        while (i < arr.size() && arr[i] <= target) {
            i *= 2;
        }
        
        // Binary search in the found range
        int left = i / 2;
        int right = min(i, (int)arr.size() - 1);
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1;
    }
    
    // Interpolation Search
    int interpolationSearch(const vector<int>& arr, int target) {
        int left = 0, right = arr.size() - 1;
        
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            if (left == right) {
                return (arr[left] == target) ? left : -1;
            }
            
            // Interpolation formula
            int pos = left + ((double)(target - arr[left]) / (arr[right] - arr[left])) * (right - left);
            
            if (arr[pos] == target) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return -1;
    }
    
    // Jump Search
    int jumpSearch(const vector<int>& arr, int target) {
        int n = arr.size();
        int step = sqrt(n);
        int prev = 0;
        
        // Jump to find the block
        while (arr[min(step, n) - 1] < target) {
            prev = step;
            step += sqrt(n);
            if (prev >= n) return -1;
        }
        
        // Linear search in the block
        while (arr[prev] < target) {
            prev++;
            if (prev == min(step, n)) return -1;
        }
        
        return (arr[prev] == target) ? prev : -1;
    }
    
    // Fibonacci Search
    int fibonacciSearch(const vector<int>& arr, int target) {
        int n = arr.size();
        
        // Generate Fibonacci numbers
        int fib2 = 0, fib1 = 1, fib = fib1 + fib2;
        
        while (fib < n) {
            fib2 = fib1;
            fib1 = fib;
            fib = fib1 + fib2;
        }
        
        int offset = -1;
        
        while (fib > 1) {
            int i = min(offset + fib2, n - 1);
            
            if (arr[i] < target) {
                fib = fib1;
                fib1 = fib2;
                fib2 = fib - fib1;
                offset = i;
            } else if (arr[i] > target) {
                fib = fib2;
                fib1 = fib1 - fib2;
                fib2 = fib - fib1;
            } else {
                return i;
            }
        }
        
        if (fib1 && offset + 1 < n && arr[offset + 1] == target) {
            return offset + 1;
        }
        
        return -1;
    }
    
    // Search in Rotated Sorted Array
    int searchRotated(const vector<int>& arr, int target) {
        int left = 0, right = arr.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) return mid;
            
            // Left half is sorted
            if (arr[left] <= arr[mid]) {
                if (target >= arr[left] && target < arr[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            // Right half is sorted
            else {
                if (target > arr[mid] && target <= arr[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        return -1;
    }
    
    // Find Peak Element
    int findPeakElement(const vector<int>& arr) {
        int left = 0, right = arr.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] > arr[mid + 1]) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
    // Find Minimum in Rotated Sorted Array
    int findMin(const vector<int>& arr) {
        int left = 0, right = arr.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] > arr[right]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left;
    }
    
    // Search for Range (first and last occurrence)
    pair<int, int> searchRange(const vector<int>& arr, int target) {
        int first = lowerBound(arr, target);
        if (first == arr.size() || arr[first] != target) {
            return {-1, -1};
        }
        
        int last = upperBound(arr, target) - 1;
        return {first, last};
    }
}

// Example usage:
/*
int main() {
    using namespace SearchingAlgorithms;
    
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    int target = 7;
    
    cout << "Binary search: " << binarySearch(arr, target) << endl;
    cout << "Lower bound: " << lowerBound(arr, target) << endl;
    cout << "Upper bound: " << upperBound(arr, target) << endl;
    cout << "Exponential search: " << exponentialSearch(arr, target) << endl;
    cout << "Jump search: " << jumpSearch(arr, target) << endl;
    cout << "Fibonacci search: " << fibonacciSearch(arr, target) << endl;
    
    // Ternary search example
    auto f = [](double x) { return -(x - 3) * (x - 3) + 10; }; // Parabola with max at x=3
    double max_x = ternarySearchMax(0, 6, f);
    cout << "Ternary search max at: " << max_x << endl;
    
    // Rotated array search
    vector<int> rotated = {4, 5, 6, 7, 0, 1, 2};
    cout << "Search in rotated array: " << searchRotated(rotated, 0) << endl;
    
    return 0;
}
*/