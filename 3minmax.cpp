// Implement Min, Max, Sum and Average operations using Parallel Reduction.

#include <iostream>
#include <omp.h>
#include <limits>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Serial implementations
int serialMin(int arr[], int n) {
    int minval = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < minval)
            minval = arr[i];
    }
    return minval;
}

int serialMax(int arr[], int n) {
    int maxval = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxval)
            maxval = arr[i];
    }
    return maxval;
}

long long serialSum(int arr[], int n) {
    long long total = 0;
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}

double serialAverage(int arr[], int n) {
    return (double) serialSum(arr, n) / n;
}

// Parallel implementations using OpenMP
int parallelMin(int arr[], int n) {
    int minval = arr[0];
    #pragma omp parallel for reduction(min:minval)
    for (int i = 0; i < n; i++) {
        if (arr[i] < minval)
            minval = arr[i];
    }
    return minval;
}

int parallelMax(int arr[], int n) {
    int maxval = arr[0];
    #pragma omp parallel for reduction(max:maxval)
    for (int i = 0; i < n; i++) {
        if (arr[i] > maxval)
            maxval = arr[i];
    }
    return maxval;
}

long long parallelSum(int arr[], int n) {
    long long total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}

double parallelAverage(int arr[], int n) {
    return static_cast<double>(parallelSum(arr, n)) / n;
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    int* arr = new int[n];
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // SERIAL
    auto start1 = high_resolution_clock::now();
    cout << "\n[Serial Execution]" << endl;
    cout << "Min: " << serialMin(arr, n) << endl;
    cout << "Max: " << serialMax(arr, n) << endl;
    cout << "Sum: " << serialSum(arr, n) << endl;
    cout << "Average: " << serialAverage(arr, n) << endl;
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    cout << "Serial Time: " << duration1.count() / 1000.0 << " ms\n";

    // PARALLEL
    auto start2 = high_resolution_clock::now();
    cout << "\n[Parallel Execution]" << endl;
    cout << "Min: " << parallelMin(arr, n) << endl;
    cout << "Max: " << parallelMax(arr, n) << endl;
    cout << "Sum: " << parallelSum(arr, n) << endl;
    cout << "Average: " << parallelAverage(arr, n) << endl;
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    cout << "Parallel Time: " << duration2.count() / 1000.0 << " ms\n";

    double speedup = duration1 / duration2;
    int numThreads = 4;
    double efficiency = speedup / numThreads;

    cout << "Speedup: " << speedup << "\n";
    cout << "Efficiency: " << efficiency << "\n";

    delete[] arr;
    return 0;
}
