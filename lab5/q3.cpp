#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;

int main() {
    // 100 million elements
    const int N = 100000000; 
    
    // Initialize vectors: a with 1.5, b with 2.5
    vector<float> a(N, 1.5f), b(N, 2.5f), c(N);

    // 1. Measure Standard Loop Time
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        c[i] = a[i] * b[i];
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> normal_time = end - start;
    cout << "Standard multiplication time: " << normal_time.count() << " seconds\n";

    // 2. Measure Vectorized Loop Time (SIMD)
    start = chrono::high_resolution_clock::now();
    #pragma omp simd
    for (int i = 0; i < N; i++) {
        c[i] = a[i] * b[i];
    }
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> simd_time = end - start;
    cout << "Vectorized multiplication time: " << simd_time.count() << " seconds\n";

    // 3. Calculate Speedup
    cout << "Speedup (normal/simd): " << normal_time.count() / simd_time.count() << "x\n";

    return 0;
}