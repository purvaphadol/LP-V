// Write a program to implement Parallel Bubble Sort using OpenMP. Use existing
// algorithms and measure the performance of sequential and parallel algorithms.

#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <random>  // For random number generation

using namespace std;
using namespace chrono;

void sequentialBubbleSort(vector<int>& arr) {
    int size = arr.size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int size = arr.size();
    for (int i = 0; i < size; i++) {
        #pragma omp parallel for
        for (int j = i % 2; j < size - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void printArray(const vector<int>& arr) {
    for (int value : arr) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    int size;
    cout << "Enter the number of elements: ";
    cin >> size;
    
    vector<int> inputArray(size);
    
    // cout << "Enter the elements:\n";
    // for (int i = 0; i < size; ++i) {
    //     cout << "Element " << i + 1 << ": ";
    //     cin >> inputArray[i];
    // }

    // Use random number generation
    random_device rd;
    mt19937 gen(rd()); // Mersenne Twister RNG
    uniform_int_distribution<> distrib(1, 5000); // Range [1, 1000]
    
    for (int i = 0; i < size; ++i) {
        inputArray[i] = distrib(gen);
    }
    

    vector<int> arraySequential = inputArray;
    vector<int> arrayParallel = inputArray;

    // Measure sequential sort time
    auto startSeq = high_resolution_clock::now();
    sequentialBubbleSort(arraySequential);
    auto endSeq = high_resolution_clock::now();
    //chrono::duration<double, milli> durationSeq = endSeq - startSeq;
    auto durationSeq = duration_cast<microseconds>(endSeq - startSeq);


    // Measure parallel sort time
    auto startPar = high_resolution_clock::now();
    parallelBubbleSort(arrayParallel);
    auto endPar = high_resolution_clock::now();
    //chrono::duration<double, milli> durationPar = endPar - startPar;
    auto durationPar = duration_cast<microseconds>(endPar - startPar);

    // Output
    cout << "\n----- Sequential Bubble Sort Output -----\n";
    cout << "Sorted Array: ";
    //printArray(arraySequential);
    cout << "Time Taken: " << durationSeq.count() << " microseconds\n";

    cout << "\n----- Parallel Bubble Sort Output -----\n";
    cout << "Sorted Array: ";
    //printArray(arrayParallel);
    cout << "Time Taken: " << durationPar.count() << " microseconds\n";

    return 0;
}
/*
Enter the number of elements: 50

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 0 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 0 microseconds
PS C:\Users\purva\Documents\Coding Practice\BE\Final> cd "c:\Users\purva\Documents\Coding Practice\BE\Final\" ; if ($?) { g++ bub.cpp -o bub -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 } ; if ($?) { .\bub }
Enter the number of elements: 100

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 0 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 0 microseconds
PS C:\Users\purva\Documents\Coding Practice\BE\Final> cd "c:\Users\purva\Documents\Coding Practice\BE\Final\" ; if ($?) { g++ bub.cpp -o bub -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 } ; if ($?) { .\bub }
Enter the number of elements: 500

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 4049 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 4942 microseconds
PS C:\Users\purva\Documents\Coding Practice\BE\Final> cd "c:\Users\purva\Documents\Coding Practice\BE\Final\" ; if ($?) { g++ bub.cpp -o bub -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 } ; if ($?) { .\bub }  
Enter the number of elements: 1000

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 12105 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 13566 microseconds
PS C:\Users\purva\Documents\Coding Practice\BE\Final> cd "c:\Users\purva\Documents\Coding Practice\BE\Final\" ; if ($?) { g++ bub.cpp -o bub -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 } ; if ($?) { .\bub }
Enter the number of elements: 2000

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 43621 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 34001 microseconds
PS C:\Users\purva\Documents\Coding Practice\BE\Final> cd "c:\Users\purva\Documents\Coding Practice\BE\Final\" ; if ($?) { g++ bub.cpp -o bub -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 } ; if ($?) { .\bub }
Enter the number of elements: 1500

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 24496 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 22947 microseconds
PS C:\Users\purva\Documents\Coding Practice\BE\Final> cd "c:\Users\purva\Documents\Coding Practice\BE\Final\" ; if ($?) { g++ bub.cpp -o bub -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 } ; if ($?) { .\bub }
Enter the number of elements: 3000

----- Sequential Bubble Sort Output -----
Sorted Array: Time Taken: 102694 microseconds

----- Parallel Bubble Sort Output -----
Sorted Array: Time Taken: 76207 microseconds*/