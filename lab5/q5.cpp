#include <iostream>
#include <chrono>
#include <vector>
#include <omp.h>
using namespace std;

int main() {
    // Array sizes to test: starting from very small to moderate
    int sizes[] = { 1, 2, 4, 128, 256, 4096 };
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < numSizes; s++) {
        int N = sizes[s];

        // Initialize vectors with dummy data
        vector<float> a(N, 2.0f), b(N, 3.0f), c(N);

        // 1. Measure Standard Loop
        auto start1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
        }
        auto end1 = chrono::high_resolution_clock::now();
        chrono::duration<double> dur1 = end1 - start1;

        // 2. Measure Vectorized Loop (SIMD)
        auto start2 = chrono::high_resolution_clock::now();
        #pragma omp simd
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
        }
        auto end2 = chrono::high_resolution_clock::now();
        chrono::duration<double> dur2 = end2 - start2;

        // Output Results
        cout << "Array size = " << N << endl;
        cout << "Standard loop time:    " << dur1.count() << " sec" << endl;
        cout << "Vectorized loop time:  " << dur2.count() << " sec" << endl;
        
        // Avoid division by zero if time is extremely small
        if (dur2.count() > 0) {
             cout << "Speedup:               " << dur1.count() / dur2.count() << "x" << endl;
        }
        cout << "------------------------------" << endl;
    }

    return 0;
}