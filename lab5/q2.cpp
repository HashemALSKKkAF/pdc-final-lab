#include <iostream>
#include <ctime> // for clock()
using namespace std;

int main(int argc, char** argv) {
    const int size = 512;
    clock_t start;
    double duration, vector_duration;
    float a[size], b[size];

    // Initialize arrays
    for (int i = 0; i < size; ++i) {
        a[i] = 1.0f * (i + 1);
        b[i] = 2.5f * (i + 1);
    }

    // Standard loop 
    start = clock();
    float total;
    for (int j = 0; j < 100000; ++j) {
        total = 0;
        for (int i = 0; i < size; ++i) {
            total += a[i] + b[i];
        }
    }
    duration = (clock() - start) / (double)CLOCKS_PER_SEC;

    // Vectorized loop with OpenMP SIMD
    start = clock();
    float vec_total;
    for (int j = 0; j < 100000; ++j) {
        vec_total = 0;
        #pragma omp simd
        for (int i = 0; i < size; ++i) {
            vec_total += a[i] + b[i];
        }
    }
    vector_duration = (clock() - start) / (double)CLOCKS_PER_SEC;

    // Output results
    cout << "The standard loop took " << duration << " seconds to complete. Total is " << total << endl;
    cout << "The vectorized loop took " << vector_duration << " seconds to complete. Total is " << vec_total << endl;

    return 0;
}