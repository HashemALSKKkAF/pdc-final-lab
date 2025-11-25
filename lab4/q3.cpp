#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Serial Matrix-Vector Multiplication
void MatVec_serial(double** A, double* B, double* C, int N) {
    for (int i = 0; i < N; i++) {
        C[i] = 0.0;
        for (int j = 0; j < N; j++) {
            C[i] += A[i][j] * B[j];
        }
    }
}

// Parallel Matrix-Vector Multiplication
void MatVec_parallel(double** A, double* B, double* C, int N) {
    // Parallelize the outer loop (rows)
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * B[j];
        }
        C[i] = sum;
    }
}

int main() {
    // Sizes defined in the lab
    int sizes[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    double **A, *B, *C;
    double t_serial, t_parallel;

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        printf("\nMatrix-Vector Size: %d x %d\n", N, N);

        // Dynamic Memory Allocation for 2D Array A
        // Note: This lab uses an array of pointers (double**) rather than a flattened 1D array.
        A = (double**)malloc(N * sizeof(double*));
        for (int i = 0; i < N; i++) {
            A[i] = (double*)malloc(N * sizeof(double));
        }

        B = (double*)malloc(N * sizeof(double));
        C = (double*)malloc(N * sizeof(double));

        // Initialize Matrix and Vector with random values
        for (int i = 0; i < N; i++) {
            B[i] = (double)rand() / RAND_MAX;
            for (int j = 0; j < N; j++) {
                A[i][j] = (double)rand() / RAND_MAX;
            }
        }

        // Measure Serial Execution Time
        t_serial = omp_get_wtime();
        MatVec_serial(A, B, C, N);
        t_serial = omp_get_wtime() - t_serial;
        printf("Serial Time: %f sec\n", t_serial);

        // Measure Parallel Execution Time with varying threads
        for (int t = 1; t <= 32; t *= 2) {
            omp_set_num_threads(t);

            t_parallel = omp_get_wtime();
            MatVec_parallel(A, B, C, N);
            t_parallel = omp_get_wtime() - t_parallel;

            // Calculate Metrics
            double speedup = t_serial / t_parallel;
            double efficiency = speedup / t;
            double cost = t_parallel * t;

            printf("Threads: %2d | Time: %f | Speedup: %.2f | Efficiency: %.2f | Cost: %.6f\n",
                   t, t_parallel, speedup, efficiency, cost);
        }

        // Free Memory (Critical for 2D arrays to avoid leaks)
        for (int i = 0; i < N; i++) free(A[i]);
        free(A); free(B); free(C);
    }

    return 0;
}