#include <stdio.h>
#include <omp.h>

int main() {
    // Define problem size
    // Note: We use static arrays to avoid stack overflow for large matrices
    const int size = 500; 
    static float a[1000][1000], b[1000][1000], c[1000][1000];
    int REPS = 1000; // Repetitions to make the execution time measurable

    // Initialize arrays
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            a[i][j] = (i + 1) * 0.5f;
            b[i][j] = (j + 1) * 0.3f + 1.0f; // +1.0f to avoid division by zero
        }
    }

    double start, end;
    double time_unvectorized, time_outer, time_inner, time_collapse;

    // --- Case 1: Unvectorized Double Loop ---
    start = omp_get_wtime();
    for (int k = 0; k < REPS; ++k) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                c[i][j] = a[i][j] / b[i][j];
            }
        }
    }
    end = omp_get_wtime();
    time_unvectorized = end - start;
    printf("Unvectorized double loop time = %f sec\n", time_unvectorized);

    // --- Case 2: Outer Loop Vectorized ---
    // Usually poor performance because 'i' jumps across memory rows
    start = omp_get_wtime();
    for (int k = 0; k < REPS; ++k) {
        #pragma omp simd
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                c[i][j] = a[i][j] / b[i][j];
            }
        }
    }
    end = omp_get_wtime();
    time_outer = end - start;
    printf("Outer loop vectorized time    = %f sec\n", time_outer);

    // --- Case 3: Inner Loop Vectorized ---
    // Good performance: 'j' iterates over contiguous memory
    start = omp_get_wtime();
    for (int k = 0; k < REPS; ++k) {
        for (int i = 0; i < size; ++i) {
            #pragma omp simd
            for (int j = 0; j < size; ++j) {
                c[i][j] = a[i][j] / b[i][j];
            }
        }
    }
    end = omp_get_wtime();
    time_inner = end - start;
    printf("Inner loop vectorized time    = %f sec\n", time_inner);

    // --- Case 4: Loop Collapse ---
    // Best potential: Treats the 2D array as one giant 1D array
    start = omp_get_wtime();
    for (int k = 0; k < REPS; ++k) {
        // Collapse(2) merges the next 2 loops into one
        #pragma omp simd collapse(2)
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                c[i][j] = a[i][j] / b[i][j];
            }
        }
    }
    end = omp_get_wtime();
    time_collapse = end - start;
    printf("Collapse(2) vectorized time   = %f sec\n", time_collapse);

    // --- Summary ---
    printf("\n--- Speed-up Summary ---\n");
    if (time_outer > 0) printf("Outer loop speed-up    = %fx\n", time_unvectorized / time_outer);
    if (time_inner > 0) printf("Inner loop speed-up    = %fx\n", time_unvectorized / time_inner);
    if (time_collapse > 0) printf("Collapse(2) speed-up   = %fx\n", time_unvectorized / time_collapse);

    return 0;
}