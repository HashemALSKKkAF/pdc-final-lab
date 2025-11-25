#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;

int main() {
    // 100 million elements
    const int N = 100000000;
    
    // Initialize vectors: a with 3.5, b with 1.5
    // Note: Division by zero would crash, so we ensure 'b' is non-zero.
    vector<float> a(N, 3.5f), b(N, 1.5f), c(N);

    // 1. Measure Standard Division Time
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        c[i] = a[i] / b[i];
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> normal_time = end - start;
    cout << "Standard division time: " << normal_time.count() << " seconds\n";

    // 2. Measure Vectorized Division Time (SIMD)
    start = chrono::high_resolution_clock::now();
    #pragma omp simd
    for (int i = 0; i < N; i++) {
        c[i] = a[i] / b[i];
    }
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> simd_time = end - start;
    cout << "Vectorized division time: " << simd_time.count() << " seconds\n";

    // 3. Calculate Speedup
    cout << "Speedup (normal/simd): " << normal_time.count() / simd_time.count() << "x\n";

    return 0;
}