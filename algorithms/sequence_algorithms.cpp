#include <bits/stdc++.h>
using namespace std;

// Core Sequence Processing Algorithms
// Note: Dynamic Programming algorithms moved to dynamic_programming.cpp
// Note: Sliding Window algorithms moved to sliding_window.cpp
// Note: String algorithms moved to string_algorithms.cpp
namespace SequenceAlgorithms {
    
    // Coordinate Compression
    vector<int> coordinateCompression(vector<int> arr) {
        vector<int> sorted_arr = arr;
        sort(sorted_arr.begin(), sorted_arr.end());
        sorted_arr.erase(unique(sorted_arr.begin(), sorted_arr.end()), sorted_arr.end());
        
        for (int& x : arr) {
            x = lower_bound(sorted_arr.begin(), sorted_arr.end(), x) - sorted_arr.begin();
        }
        
        return arr;
    }
    
    // Count Inversions using Merge Sort
    long long countInversions(vector<int>& arr) {
        return mergeAndCount(arr, 0, arr.size() - 1);
    }
    
    // Helper function for counting inversions
    long long mergeAndCount(vector<int>& arr, int left, int right) {
        long long invCount = 0;
        if (left < right) {
            int mid = left + (right - left) / 2;
            
            invCount += mergeAndCount(arr, left, mid);
            invCount += mergeAndCount(arr, mid + 1, right);
            invCount += mergeInversions(arr, left, mid, right);
        }
        return invCount;
    }
    
    long long mergeInversions(vector<int>& arr, int left, int mid, int right) {
        vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        long long invCount = 0;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
                invCount += (mid - i + 1);
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (i = left, k = 0; i <= right; i++, k++) {
            arr[i] = temp[k];
        }
        
        return invCount;
    }
    
    // Find all subarrays with given sum
    vector<pair<int, int>> findSubarraysWithSum(const vector<int>& arr, int targetSum) {
        vector<pair<int, int>> result;
        unordered_map<int, vector<int>> prefixSumMap;
        prefixSumMap[0].push_back(-1);
        
        int currentSum = 0;
        for (int i = 0; i < (int)arr.size(); i++) {
            currentSum += arr[i];
            
            if (prefixSumMap.count(currentSum - targetSum)) {
                for (int startIdx : prefixSumMap[currentSum - targetSum]) {
                    result.push_back({startIdx + 1, i});
                }
            }
            
            prefixSumMap[currentSum].push_back(i);
        }
        
        return result;
    }
    
    // Longest subarray with sum K
    int longestSubarrayWithSum(const vector<int>& arr, int k) {
        unordered_map<int, int> prefixSumMap;
        prefixSumMap[0] = -1;
        
        int maxLen = 0, currentSum = 0;
        for (int i = 0; i < arr.size(); i++) {
            currentSum += arr[i];
            
            if (prefixSumMap.count(currentSum - k)) {
                maxLen = max(maxLen, i - prefixSumMap[currentSum - k]);
            }
            
            if (!prefixSumMap.count(currentSum)) {
                prefixSumMap[currentSum] = i;
            }
        }
        
        return maxLen;
    }
    
    // Check if array is sorted
    bool isSorted(const vector<int>& arr) {
        for (int i = 1; i < (int)arr.size(); i++) {
            if (arr[i] < arr[i - 1]) return false;
        }
        return true;
    }
    
    // Remove duplicates from sorted array
    int removeDuplicates(vector<int>& arr) {
        if (arr.empty()) return 0;
        
        int writeIndex = 1;
        for (int i = 1; i < (int)arr.size(); i++) {
            if (arr[i] != arr[i - 1]) {
                arr[writeIndex++] = arr[i];
            }
        }
        
        return writeIndex;
    }
}

// Example usage:
/*
int main() {
    using namespace SequenceAlgorithms;
    
    vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    vector<int> compressed = coordinateCompression(arr);
    // compressed will be {2, 0, 3, 0, 4, 7, 1, 5}
    
    vector<int> inversions = {2, 4, 1, 3, 5};
    cout << "Inversion count: " << countInversions(inversions) << endl;
    
    vector<int> nums = {1, 2, 3, 2, 1, 4, 5};
    vector<pair<int, int>> subarrays = findSubarraysWithSum(nums, 5);
    
    vector<int> arr2 = {1, -1, 0, 1, 1, 0, 1};
    cout << "Longest subarray with sum 2: " << longestSubarrayWithSum(arr2, 2) << endl;
    
    vector<int> sorted = {1, 1, 2, 3, 3, 4, 5};
    cout << "Is sorted: " << isSorted(sorted) << endl;
    int newSize = removeDuplicates(sorted);
    // sorted now contains {1, 2, 3, 4, 5} with newSize = 5
    
    return 0;
}
*/