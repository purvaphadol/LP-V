#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <cstdlib>  // for rand()
#include <ctime>    // for seeding rand()
using namespace std;

#define MAX 100

void sequentialMatrixVectorMultiply(int A[MAX][MAX], const vector<int>& x, vector<int>& result, int n, int m) {
    for (int i = 0; i < n; ++i) {
        result[i] = 0;
        for (int j = 0; j < m; ++j)
            result[i] += A[i][j] * x[j];
    }
}

void parallelMatrixVectorMultiply(int A[MAX][MAX], const vector<int>& x, vector<int>& result, int n, int m) {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        result[i] = 0;
        for (int j = 0; j < m; ++j)
            result[i] += A[i][j] * x[j];
    }
}

void printVector(const vector<int>& result) {
    for (auto val : result)
        cout << val << " ";
    cout << endl;
}

void printMatrix(int A[MAX][MAX], int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            cout << A[i][j] << " ";
        cout << endl;
    }
}

int main() {
    int n, m;
    srand(time(0));  // Seed the random generator

    cout << "Enter dimensions (Matrix A: n x m, Vector x: m x 1)\n";
    cout << "Enter n (rows): ";
    cin >> n;
    cout << "Enter m (cols): ";
    cin >> m;

    int A[MAX][MAX];
    vector<int> x(m);
    vector<int> resultSeq(n, 0), resultPar(n, 0);

    // Generate random matrix A and vector x
    cout << "\nRandom Matrix A:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            A[i][j] = rand() % 50; // values 0-9

    printMatrix(A, n, m);

    cout << "\nRandom Vector x:\n";
    for (int i = 0; i < m; ++i) {
        x[i] = rand() % 10;
        cout << x[i] << " ";
    }
    cout << endl;

    auto startSeq = chrono::high_resolution_clock::now();
    sequentialMatrixVectorMultiply(A, x, resultSeq, n, m);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;

    auto startPar = chrono::high_resolution_clock::now();
    parallelMatrixVectorMultiply(A, x, resultPar, n, m);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;

    cout << "\n----- Sequential Matrix-Vector Multiplication Output -----\n";
    printVector(resultSeq);
    cout << "Time Taken: " << durationSeq.count() << " milliseconds\n";

    cout << "\n----- Parallel Matrix-Vector Multiplication Output -----\n";
    printVector(resultPar);
    cout << "Time Taken: " << durationPar.count() << " milliseconds\n";

    return 0;
}
