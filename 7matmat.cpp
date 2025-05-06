#include <iostream>
#include <omp.h>
#include <chrono>
#include <random>
using namespace std;

#define MAX 100

void sequentialMatrixMultiply(int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX], int n, int m, int p) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < p; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

void parallelMatrixMultiply(int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX], int n, int m, int p) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < p; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

void printMatrix(int M[MAX][MAX], int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            cout << M[i][j] << " ";
        cout << endl;
    }
}

int main() {
    int n, m, p;
    int A[MAX][MAX], B[MAX][MAX], C_seq[MAX][MAX] = {}, C_par[MAX][MAX] = {};

    cout << "Enter dimensions (A: n x p, B: p x m)\n";
    cout << "Enter n (rows of A): ";
    cin >> n;
    cout << "Enter p (cols of A / rows of B): ";
    cin >> p;
    cout << "Enter m (cols of B): ";
    cin >> m;

    // Random number generation setup
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 10);  // values between 1 and 10

    // Fill A and B with random values
    cout << "\nMatrix A:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < p; ++j) {
            A[i][j] = distrib(gen);
            cout << A[i][j] << " ";
            if (j == p - 1) cout << endl;
        }

    cout << "\nMatrix B:\n";
    for (int i = 0; i < p; ++i)
        for (int j = 0; j < m; ++j) {
            B[i][j] = distrib(gen);
            cout << B[i][j] << " ";
            if (j == m - 1) cout << endl;
        }

    auto startSeq = chrono::high_resolution_clock::now();
    sequentialMatrixMultiply(A, B, C_seq, n, m, p);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;

    auto startPar = chrono::high_resolution_clock::now();
    parallelMatrixMultiply(A, B, C_par, n, m, p);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;

    cout << "\n----- Sequential Matrix-Matrix Multiplication Output -----\n";
    printMatrix(C_seq, n, m);
    cout << "Time Taken: " << durationSeq.count() << " milliseconds\n";

    cout << "\n----- Parallel Matrix-Matrix Multiplication Output -----\n";
    printMatrix(C_par, n, m);
    cout << "Time Taken: " << durationPar.count() << " milliseconds\n";

    return 0;
}
