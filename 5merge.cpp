#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <random>
using namespace std;

// Merge two sorted parts of the array
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (size_t k = 0; k < temp.size(); ++k)
        arr[left + k] = temp[k];
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right-left) / 2;
        //int mid = (left + right) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

// Print array elements
void printArray(const vector<int>& arr) {
    for (const int& val : arr)
        cout << val << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> inputArray(n);
    //vector<int> arr(n), arrCopy(n);
    // cout << "Enter elements:\n";
    // for (int i = 0; i < n; ++i) {
    //     cin >> arr[i];
    //     arrCopy[i] = arr[i];
    // }

    random_device rd;
    mt19937 gen(rd()); // Mersenne Twister RNG
    uniform_int_distribution<> distrib(1, 5000); // Range [1, 1000]
    
    for (int i = 0; i < n; ++i) {
        inputArray[i] = distrib(gen);
    }
    vector<int> arr = inputArray;
    vector<int> arrCopy = inputArray;

    // Sequential Merge Sort
    auto startSeq = chrono::high_resolution_clock::now();
    sequentialMergeSort(arr, 0, n - 1);
    auto endSeq = chrono::high_resolution_clock::now();

    // Parallel Merge Sort
    auto startPar = chrono::high_resolution_clock::now();
    parallelMergeSort(arrCopy, 0, n - 1);
    auto endPar = chrono::high_resolution_clock::now();

    auto timeSeq = chrono::duration<double, milli>(endSeq - startSeq).count();
    auto timePar = chrono::duration<double, milli>(endPar - startPar).count();

    //int numThreads = omp_get_max_threads();
    int numThreads = 4;
    double speedup = timeSeq / timePar;
    double efficiency = speedup / numThreads;

    cout << "\n--- Sequential Merge Sort ---\n";
    printArray(arr);
    cout << "Time: " << timeSeq << " ms\n";

    cout << "\n--- Parallel Merge Sort ---\n";
    printArray(arrCopy);
    cout << "Time: " << timePar << " ms\n";

    cout << "\n--- Performance Metrics ---\n";
    cout << "Number of Threads: " << numThreads << endl;
    cout << "Speedup: " << speedup << endl;
    cout << "Efficiency: " << efficiency * 100 << " %\n";

    return 0;
}
