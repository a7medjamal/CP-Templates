#include <bits/stdc++.h>
using namespace std;

// Sorting Algorithms
namespace SortingAlgorithms {
    
    // Merge Sort
    void merge(vector<int>& arr, int left, int mid, int right) {
        vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (i = left, k = 0; i <= right; i++, k++) {
            arr[i] = temp[k];
        }
    }
    
    void mergeSort(vector<int>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
    
    void mergeSort(vector<int>& arr) {
        mergeSort(arr, 0, arr.size() - 1);
    }
    
    // Quick Sort
    int partition(vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }
    
    void quickSort(vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
    
    void quickSort(vector<int>& arr) {
        quickSort(arr, 0, arr.size() - 1);
    }
    
    // Randomized Quick Sort
    int randomizedPartition(vector<int>& arr, int low, int high) {
        int random = low + rand() % (high - low + 1);
        swap(arr[random], arr[high]);
        return partition(arr, low, high);
    }
    
    void randomizedQuickSort(vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = randomizedPartition(arr, low, high);
            randomizedQuickSort(arr, low, pi - 1);
            randomizedQuickSort(arr, pi + 1, high);
        }
    }
    
    void randomizedQuickSort(vector<int>& arr) {
        srand(time(nullptr));
        randomizedQuickSort(arr, 0, arr.size() - 1);
    }
    
    // Heap Sort
    void heapify(vector<int>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }
        
        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }
    
    void heapSort(vector<int>& arr) {
        int n = arr.size();
        
        // Build heap
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }
        
        // Extract elements from heap
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }
    
    // Counting Sort
    void countingSort(vector<int>& arr) {
        if (arr.empty()) return;
        
        int maxVal = *max_element(arr.begin(), arr.end());
        int minVal = *min_element(arr.begin(), arr.end());
        int range = maxVal - minVal + 1;
        
        vector<int> count(range, 0);
        vector<int> output(arr.size());
        
        // Count occurrences
        for (int num : arr) {
            count[num - minVal]++;
        }
        
        // Cumulative count
        for (int i = 1; i < range; i++) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = (int)arr.size() - 1; i >= 0; i--) {
            output[count[arr[i] - minVal] - 1] = arr[i];
            count[arr[i] - minVal]--;
        }
        
        arr = output;
    }
    
    // Radix Sort
    void countingSortForRadix(vector<int>& arr, int exp) {
        vector<int> output(arr.size());
        vector<int> count(10, 0);
        
        for (int num : arr) {
            count[(num / exp) % 10]++;
        }
        
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }
        
        for (int i = (int)arr.size() - 1; i >= 0; i--) {
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }
        
        arr = output;
    }
    
    void radixSort(vector<int>& arr) {
        if (arr.empty()) return;
        
        int maxVal = *max_element(arr.begin(), arr.end());
        
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSortForRadix(arr, exp);
        }
    }
    
    // Bucket Sort
    void bucketSort(vector<float>& arr) {
        int n = arr.size();
        vector<vector<float>> buckets(n);
        
        // Put array elements in different buckets
        for (float num : arr) {
            int bi = n * num;
            buckets[bi].push_back(num);
        }
        
        // Sort individual buckets
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end());
        }
        
        // Concatenate all buckets
        int index = 0;
        for (const auto& bucket : buckets) {
            for (float num : bucket) {
                arr[index++] = num;
            }
        }
    }
    
    // Shell Sort
    void shellSort(vector<int>& arr) {
        int n = arr.size();
        
        // Start with a big gap, then reduce the gap
        for (int gap = n / 2; gap > 0; gap /= 2) {
            // Do a gapped insertion sort
            for (int i = gap; i < n; i++) {
                int temp = arr[i];
                int j;
                
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap];
                }
                
                arr[j] = temp;
            }
        }
    }
    
    // Insertion Sort
    void insertionSort(vector<int>& arr) {
        int n = arr.size();
        
        for (int i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;
            
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            
            arr[j + 1] = key;
        }
    }
    
    // Selection Sort
    void selectionSort(vector<int>& arr) {
        int n = arr.size();
        
        for (int i = 0; i < n - 1; i++) {
            int minIdx = i;
            
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }
            
            swap(arr[minIdx], arr[i]);
        }
    }
    
    // Bubble Sort
    void bubbleSort(vector<int>& arr) {
        int n = arr.size();
        
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            
            if (!swapped) break;
        }
    }
    
    // Cocktail Shaker Sort
    void cocktailSort(vector<int>& arr) {
        int n = arr.size();
        bool swapped = true;
        int start = 0, end = n - 1;
        
        while (swapped) {
            swapped = false;
            
            // Forward pass
            for (int i = start; i < end; i++) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            
            if (!swapped) break;
            
            end--;
            swapped = false;
            
            // Backward pass
            for (int i = end - 1; i >= start; i--) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            
            start++;
        }
    }
    
    // Comb Sort
    void combSort(vector<int>& arr) {
        int n = arr.size();
        int gap = n;
        bool swapped = true;
        
        while (gap != 1 || swapped) {
            gap = (gap * 10) / 13;
            if (gap < 1) gap = 1;
            
            swapped = false;
            
            for (int i = 0; i < n - gap; i++) {
                if (arr[i] > arr[i + gap]) {
                    swap(arr[i], arr[i + gap]);
                    swapped = true;
                }
            }
        }
    }
    
    // Cycle Sort
    int cycleSort(vector<int>& arr) {
        int writes = 0;
        int n = arr.size();
        
        for (int cycleStart = 0; cycleStart <= n - 2; cycleStart++) {
            int item = arr[cycleStart];
            int pos = cycleStart;
            
            for (int i = cycleStart + 1; i < n; i++) {
                if (arr[i] < item) pos++;
            }
            
            if (pos == cycleStart) continue;
            
            while (item == arr[pos]) pos++;
            
            if (pos != cycleStart) {
                swap(item, arr[pos]);
                writes++;
            }
            
            while (pos != cycleStart) {
                pos = cycleStart;
                
                for (int i = cycleStart + 1; i < n; i++) {
                    if (arr[i] < item) pos++;
                }
                
                while (item == arr[pos]) pos++;
                
                if (item != arr[pos]) {
                    swap(item, arr[pos]);
                    writes++;
                }
            }
        }
        
        return writes;
    }
    
    // Pancake Sort
    int findMax(const vector<int>& arr, int n) {
        int mi = 0;
        for (int i = 0; i < n; i++) {
            if (arr[i] > arr[mi]) mi = i;
        }
        return mi;
    }
    
    void flip(vector<int>& arr, int i) {
        reverse(arr.begin(), arr.begin() + i + 1);
    }
    
    void pancakeSort(vector<int>& arr) {
        int n = arr.size();
        
        for (int currSize = n; currSize > 1; currSize--) {
            int mi = findMax(arr, currSize);
            
            if (mi != currSize - 1) {
                flip(arr, mi);
                flip(arr, currSize - 1);
            }
        }
    }
    
    // Inversion Count using Merge Sort
    long long mergeAndCount(vector<int>& arr, int left, int mid, int right) {
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
    
    long long mergeSortAndCount(vector<int>& arr, int left, int right) {
        long long invCount = 0;
        if (left < right) {
            int mid = left + (right - left) / 2;
            invCount += mergeSortAndCount(arr, left, mid);
            invCount += mergeSortAndCount(arr, mid + 1, right);
            invCount += mergeAndCount(arr, left, mid, right);
        }
        return invCount;
    }
    
    long long countInversions(vector<int> arr) {
        return mergeSortAndCount(arr, 0, arr.size() - 1);
    }
}

// Example usage:
/*
int main() {
    using namespace SortingAlgorithms;
    
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    
    cout << "Original array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    
    // Test different sorting algorithms
    vector<int> temp = arr;
    
    mergeSort(temp);
    cout << "Merge sort: ";
    for (int x : temp) cout << x << " ";
    cout << endl;
    
    temp = arr;
    quickSort(temp);
    cout << "Quick sort: ";
    for (int x : temp) cout << x << " ";
    cout << endl;
    
    temp = arr;
    heapSort(temp);
    cout << "Heap sort: ";
    for (int x : temp) cout << x << " ";
    cout << endl;
    
    temp = arr;
    countingSort(temp);
    cout << "Counting sort: ";
    for (int x : temp) cout << x << " ";
    cout << endl;
    
    // Count inversions
    cout << "Inversion count: " << countInversions(arr) << endl;
    
    return 0;
}
*/