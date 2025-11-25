#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Serial Vector Addition
void VecAdd_serial(double* a, double* b, double* c, int N) {
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
}

// Parallel Vector Addition using OpenMP
void VecAdd_parallel(double* a, double* b, double* c, int N) {
    // #pragma omp parallel for automatically splits the loop iterations among threads
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    // Problem sizes defined in the lab
    int sizes[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    double *a, *b, *c;
    double t_serial, t_parallel;

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        printf("\nVector Size: %d\n", N);

        // Allocate memory
        a = (double*)malloc(sizeof(double) * N);
        b = (double*)malloc(sizeof(double) * N);
        c = (double*)malloc(sizeof(double) * N);

        // Initialize vectors with random values
        for (int i = 0; i < N; i++) {
            a[i] = (double)rand() / RAND_MAX;
            b[i] = (double)rand() / RAND_MAX;
        }

        // Measure Serial Time
        t_serial = omp_get_wtime();
        VecAdd_serial(a, b, c, N);
        t_serial = omp_get_wtime() - t_serial;
        printf("Serial Time: %f sec\n", t_serial);

        // Measure Parallel Time with varying threads
        for (int t = 1; t <= 32; t *= 2) {
            omp_set_num_threads(t);

            t_parallel = omp_get_wtime();
            VecAdd_parallel(a, b, c, N);
            t_parallel = omp_get_wtime() - t_parallel;

            // Calculate Metrics
            double speedup = t_serial / t_parallel;
            double efficiency = speedup / t;
            double cost = t_parallel * t;

            printf("Threads: %2d | Time: %f | Speedup: %.2f | Efficiency: %.2f | Cost: %.6f\n",
                   t, t_parallel, speedup, efficiency, cost);
        }

        // Free memory
        free(a); free(b); free(c);
    }

    return 0;
}