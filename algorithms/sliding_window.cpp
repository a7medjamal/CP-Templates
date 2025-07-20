#include <bits/stdc++.h>
using namespace std;

// Sliding Window and Two Pointer Algorithms
namespace SlidingWindow {
    
    // Maximum Subarray Sum (Kadane's Algorithm)
    long long maxSubarraySum(const vector<int>& arr) {
        if (arr.empty()) return 0;
        
        long long maxSum = arr[0];
        long long currentSum = arr[0];
        
        for (int i = 1; i < arr.size(); i++) {
            currentSum = max((long long)arr[i], currentSum + arr[i]);
            maxSum = max(maxSum, currentSum);
        }
        
        return maxSum;
    }
    
    // Maximum Subarray Sum with indices
    struct SubarrayResult {
        long long sum;
        int start, end;
    };
    
    SubarrayResult maxSubarrayWithIndices(const vector<int>& arr) {
        if (arr.empty()) return {0, -1, -1};
        
        long long maxSum = arr[0];
        long long currentSum = arr[0];
        int start = 0, end = 0, tempStart = 0;
        
        for (int i = 1; i < arr.size(); i++) {
            if (currentSum < 0) {
                currentSum = arr[i];
                tempStart = i;
            } else {
                currentSum += arr[i];
            }
            
            if (currentSum > maxSum) {
                maxSum = currentSum;
                start = tempStart;
                end = i;
            }
        }
        
        return {maxSum, start, end};
    }
    
    // Maximum Product Subarray
    long long maxProductSubarray(const vector<int>& arr) {
        if (arr.empty()) return 0;
        
        long long maxProduct = arr[0];
        long long minProduct = arr[0];
        long long result = arr[0];
        
        for (int i = 1; i < arr.size(); i++) {
            if (arr[i] < 0) {
                swap(maxProduct, minProduct);
            }
            
            maxProduct = max((long long)arr[i], maxProduct * arr[i]);
            minProduct = min((long long)arr[i], minProduct * arr[i]);
            
            result = max(result, maxProduct);
        }
        
        return result;
    }
    
    // Sliding Window Maximum
    vector<int> slidingWindowMaximum(const vector<int>& arr, int k) {
        vector<int> result;
        deque<int> dq; // Store indices
        
        for (int i = 0; i < arr.size(); i++) {
            // Remove elements outside current window
            while (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }
            
            // Remove elements smaller than current element
            while (!dq.empty() && arr[dq.back()] <= arr[i]) {
                dq.pop_back();
            }
            
            dq.push_back(i);
            
            // Add to result if window is complete
            if (i >= k - 1) {
                result.push_back(arr[dq.front()]);
            }
        }
        
        return result;
    }
    
    // Sliding Window Minimum
    vector<int> slidingWindowMinimum(const vector<int>& arr, int k) {
        vector<int> result;
        deque<int> dq; // Store indices
        
        for (int i = 0; i < arr.size(); i++) {
            // Remove elements outside current window
            while (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }
            
            // Remove elements greater than current element
            while (!dq.empty() && arr[dq.back()] >= arr[i]) {
                dq.pop_back();
            }
            
            dq.push_back(i);
            
            // Add to result if window is complete
            if (i >= k - 1) {
                result.push_back(arr[dq.front()]);
            }
        }
        
        return result;
    }
    
    // Next Greater Element
    vector<int> nextGreaterElement(const vector<int>& arr) {
        vector<int> result(arr.size(), -1);
        stack<int> st;
        
        for (int i = 0; i < arr.size(); i++) {
            while (!st.empty() && arr[st.top()] < arr[i]) {
                result[st.top()] = arr[i];
                st.pop();
            }
            st.push(i);
        }
        
        return result;
    }
    
    // Previous Greater Element
    vector<int> previousGreaterElement(const vector<int>& arr) {
        vector<int> result(arr.size(), -1);
        stack<int> st;
        
        for (int i = arr.size() - 1; i >= 0; i--) {
            while (!st.empty() && arr[st.top()] < arr[i]) {
                result[st.top()] = arr[i];
                st.pop();
            }
            st.push(i);
        }
        
        return result;
    }
    
    // Next Smaller Element
    vector<int> nextSmallerElement(const vector<int>& arr) {
        vector<int> result(arr.size(), -1);
        stack<int> st;
        
        for (int i = 0; i < arr.size(); i++) {
            while (!st.empty() && arr[st.top()] > arr[i]) {
                result[st.top()] = arr[i];
                st.pop();
            }
            st.push(i);
        }
        
        return result;
    }
    
    // Previous Smaller Element
    vector<int> previousSmallerElement(const vector<int>& arr) {
        vector<int> result(arr.size(), -1);
        stack<int> st;
        
        for (int i = arr.size() - 1; i >= 0; i--) {
            while (!st.empty() && arr[st.top()] > arr[i]) {
                result[st.top()] = arr[i];
                st.pop();
            }
            st.push(i);
        }
        
        return result;
    }
    
    // Minimum Window Substring
    string minWindow(const string& s, const string& t) {
        if (s.empty() || t.empty()) return "";
        
        unordered_map<char, int> dictT;
        for (char c : t) dictT[c]++;
        
        int required = dictT.size();
        int left = 0, right = 0;
        int formed = 0;
        
        unordered_map<char, int> windowCounts;
        
        int minLen = INT_MAX, minLeft = 0;
        
        while (right < s.length()) {
            char c = s[right];
            windowCounts[c]++;
            
            if (dictT.count(c) && windowCounts[c] == dictT[c]) {
                formed++;
            }
            
            while (left <= right && formed == required) {
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minLeft = left;
                }
                
                char leftChar = s[left];
                windowCounts[leftChar]--;
                if (dictT.count(leftChar) && windowCounts[leftChar] < dictT[leftChar]) {
                    formed--;
                }
                
                left++;
            }
            
            right++;
        }
        
        return minLen == INT_MAX ? "" : s.substr(minLeft, minLen);
    }
    
    // Longest Substring Without Repeating Characters
    int lengthOfLongestSubstring(const string& s) {
        unordered_set<char> charSet;
        int left = 0, maxLen = 0;
        
        for (int right = 0; right < s.length(); right++) {
            while (charSet.count(s[right])) {
                charSet.erase(s[left]);
                left++;
            }
            
            charSet.insert(s[right]);
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
    
    // Longest Substring with At Most K Distinct Characters
    int lengthOfLongestSubstringKDistinct(const string& s, int k) {
        if (k == 0) return 0;
        
        unordered_map<char, int> charCount;
        int left = 0, maxLen = 0;
        
        for (int right = 0; right < s.length(); right++) {
            charCount[s[right]]++;
            
            while (charCount.size() > k) {
                charCount[s[left]]--;
                if (charCount[s[left]] == 0) {
                    charCount.erase(s[left]);
                }
                left++;
            }
            
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
    
    // Subarray Sum Equals K
    int subarraySum(const vector<int>& nums, int k) {
        unordered_map<int, int> prefixSum;
        prefixSum[0] = 1;
        
        int count = 0, sum = 0;
        for (int num : nums) {
            sum += num;
            if (prefixSum.count(sum - k)) {
                count += prefixSum[sum - k];
            }
            prefixSum[sum]++;
        }
        
        return count;
    }
}

// Two Pointer Algorithms
namespace TwoPointers {
    
    // Two Sum in sorted array
    pair<int, int> twoSum(const vector<int>& arr, int target) {
        int left = 0, right = arr.size() - 1;
        
        while (left < right) {
            int sum = arr[left] + arr[right];
            if (sum == target) return {left, right};
            else if (sum < target) left++;
            else right--;
        }
        
        return {-1, -1}; // Not found
    }
    
    // Three Sum
    vector<vector<int>> threeSum(vector<int>& nums, int target = 0) {
        vector<vector<int>> result;
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < nums.size() - 2; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            int left = i + 1, right = nums.size() - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == target) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < target) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        
        return result;
    }
    
    // Container with most water
    int maxArea(const vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int maxWater = 0;
        
        while (left < right) {
            int water = min(height[left], height[right]) * (right - left);
            maxWater = max(maxWater, water);
            
            if (height[left] < height[right]) left++;
            else right--;
        }
        
        return maxWater;
    }
    
    // Trapping Rain Water
    int trap(const vector<int>& height) {
        if (height.empty()) return 0;
        
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int water = 0;
        
        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= leftMax) {
                    leftMax = height[left];
                } else {
                    water += leftMax - height[left];
                }
                left++;
            } else {
                if (height[right] >= rightMax) {
                    rightMax = height[right];
                } else {
                    water += rightMax - height[right];
                }
                right--;
            }
        }
        
        return water;
    }
    
    // Remove Duplicates from Sorted Array
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;
        
        int slow = 0;
        for (int fast = 1; fast < nums.size(); fast++) {
            if (nums[fast] != nums[slow]) {
                slow++;
                nums[slow] = nums[fast];
            }
        }
        
        return slow + 1;
    }
    
    // Move Zeros
    void moveZeroes(vector<int>& nums) {
        int slow = 0;
        for (int fast = 0; fast < nums.size(); fast++) {
            if (nums[fast] != 0) {
                swap(nums[slow], nums[fast]);
                slow++;
            }
        }
    }
    
    // Partition Array
    int partition(vector<int>& nums, int pivot) {
        int left = 0, right = nums.size() - 1;
        
        while (left <= right) {
            while (left <= right && nums[left] < pivot) left++;
            while (left <= right && nums[right] >= pivot) right--;
            
            if (left < right) {
                swap(nums[left], nums[right]);
                left++;
                right--;
            }
        }
        
        return left;
    }
}

// Example usage:
/*
int main() {
    using namespace SlidingWindow;
    using namespace TwoPointers;
    
    vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << "Max subarray sum: " << maxSubarraySum(arr) << endl;
    
    auto result = maxSubarrayWithIndices(arr);
    cout << "Max subarray: sum=" << result.sum << ", start=" << result.start << ", end=" << result.end << endl;
    
    vector<int> arr2 = {1, 3, -1, -3, 5, 3, 6, 7};
    vector<int> window_max = slidingWindowMaximum(arr2, 3);
    cout << "Sliding window maximum (k=3): ";
    for (int x : window_max) cout << x << " ";
    cout << endl;
    
    vector<int> heights = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "Container with most water: " << maxArea(heights) << endl;
    
    vector<int> rain = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Trapped rain water: " << trap(rain) << endl;
    
    return 0;
}
*/