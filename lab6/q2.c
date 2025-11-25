#include <stdio.h>
#include <omp.h>

#define SIZE 10000

// Global arrays
float a[SIZE], b[SIZE], c[SIZE];

// 1. Standard Function (Scalar)
float simple_function(float a, float b) {
    float x = a * a;
    float y = b * b;
    return (x - y) / (x + y);
}

// 2. Vectorized Function Declaration
// #pragma omp declare simd tells the compiler to create a version of this 
// function that can process multiple arguments (vector lanes) simultaneously.
#pragma omp declare simd
float vector_function(float a, float b) {
    float x = a * a;
    float y = b * b;
    return (x - y) / (x + y);
}

int main() {
    // Initialization
    for (int i = 0; i < SIZE; ++i) {
        a[i] = i * 1.0f;
        b[i] = i * 2.0f + 1;
    }

    double start, end;
    double time_scalar, time_vector;

    // --- Measure Scalar Execution ---
    start = omp_get_wtime();
    // Loop 1000 times to increase computation load for measurable time
    for (int j = 0; j < 1000; ++j) {
        for (int i = 0; i < SIZE; ++i) {
            c[i] = simple_function(a[i], b[i]);
        }
    }
    end = omp_get_wtime();
    time_scalar = end - start;

    // --- Measure Vectorized Execution ---
    start = omp_get_wtime();
    for (int j = 0; j < 1000; ++j) {
        // Use SIMD for the loop, calling the vector-declared function
        #pragma omp simd
        for (int i = 0; i < SIZE; ++i) {
            c[i] = vector_function(a[i], b[i]);
        }
    }
    end = omp_get_wtime();
    time_vector = end - start;

    // --- Output Results ---
    printf("Scalar function time    = %f sec\n", time_scalar);
    printf("Vectorized SIMD time    = %f sec\n", time_vector);
    
    // Prevent division by zero if time is too fast
    if (time_vector > 0)
        printf("Speed-up (scalar / simd) = %fx\n", time_scalar / time_vector);

    return 0;
}