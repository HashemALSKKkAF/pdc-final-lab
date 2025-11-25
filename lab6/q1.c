#include <stdio.h>
#include <omp.h>

int main() {
    int size = 500;
    float a[500], b[500];
    float total = 0.0f;
    float red_total = 0.0f;

    // Initialize arrays
    for (int i = 0; i < size; ++i) {
        a[i] = i * 1.0f;
        b[i] = i * 2.0f;
    }

    // 1. Non-Vectorized (Standard) Summation
    for (int i = 0; i < size; ++i) {
        total += a[i] + b[i];
    }
    printf("Non-Vectorized total = %f\n", total);

    // 2. Vectorized (SIMD) Summation with Reduction
    // The 'reduction(+:red_total)' clause tells OpenMP to give each vector lane 
    // its own private copy of 'red_total' and sum them up at the end.
    #pragma omp simd reduction(+:red_total)
    for (int i = 0; i < size; ++i) {
        red_total += a[i] + b[i];
    }
    printf("Vectorized (SIMD) total = %f\n", red_total);

    return 0;
}