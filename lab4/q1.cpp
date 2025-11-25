#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Serial Matrix Multiplication
void MatMul(double* A, double* B, double* C, int n) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double dot = 0.0;
            for (k = 0; k < n; k++) {
                dot += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = dot;
        }
    }
}

// Parallel Matrix Multiplication using OpenMP
void MatMul_omp(double* A, double* B, double* C, int n) {
    int i, j, k;
    // Parallelize the outer loop. 
    // j and k must be private to each thread to avoid race conditions.
    #pragma omp parallel for private(j, k)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double dot = 0.0;
            for (k = 0; k < n; k++) {
                dot += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = dot;
        }
    }
}

int main() {
    int i, n;
    double *A, *B, *C;
    double dtime_serial, dtime_parallel;

    // Array of problem sizes as defined in the lab instructions
    int sizes[] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        n = sizes[s];
        printf("\nMatrix Size: %d x %d\n", n, n);

        // Dynamic memory allocation for matrices (flattened 1D arrays)
        A = (double*)malloc(sizeof(double) * n * n);
        B = (double*)malloc(sizeof(double) * n * n);
        C = (double*)malloc(sizeof(double) * n * n);

        // Initialize matrices with random values
        // Note: Using a single loop is more efficient for initialization
        for (i = 0; i < n * n; i++) {
            A[i] = (double)rand() / RAND_MAX;
            B[i] = (double)rand() / RAND_MAX;
        }

        // Measure Serial Execution Time
        dtime_serial = omp_get_wtime();
        MatMul(A, B, C, n);
        dtime_serial = omp_get_wtime() - dtime_serial;
        printf("Serial Time: %f sec\n", dtime_serial);

        // Measure Parallel Execution Time with varying threads
        // Loop for threads: 1, 2, 4, 8, 16, 32
        for (int t = 1; t <= 32; t *= 2) {
            omp_set_num_threads(t);

            dtime_parallel = omp_get_wtime();
            MatMul_omp(A, B, C, n);
            dtime_parallel = omp_get_wtime() - dtime_parallel;

            // Calculate Metrics
            double speedup = dtime_serial / dtime_parallel;
            double efficiency = speedup / t;
            double cost = dtime_parallel * t;

            printf("Threads: %2d | Time: %f | Speedup: %.2f | Efficiency: %.2f | Cost: %.2f\n",
                   t, dtime_parallel, speedup, efficiency, cost);
        }

        // Free allocated memory to prevent leaks
        free(A); free(B); free(C);
    }

    return 0;
}