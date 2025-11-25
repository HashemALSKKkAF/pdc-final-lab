#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Serial Factorial Calculation
long double factorial_serial(int N) {
    long double fact = 1.0;
    for (int i = 1; i <= N; i++) {
        fact *= i;
    }
    return fact;
}

// Parallel Factorial Calculation using Reduction
long double factorial_parallel(int N) {
    long double fact = 1.0;
    
    // Use reduction to safely multiply 'fact' across threads
    #pragma omp parallel for reduction(*:fact)
    for (int i = 1; i <= N; i++) {
        fact *= i;
    }
    return fact;
}

int main() {
    // Problem sizes as defined in the screenshot
    int sizes[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    double t_serial, t_parallel;
    long double result; // Note: 'long double' is used to hold large values

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        printf("\nFactorial of N = %d\n", N);

        // Measure Serial Time
        t_serial = omp_get_wtime();
        result = factorial_serial(N);
        t_serial = omp_get_wtime() - t_serial;
        printf("Serial Time: %f sec\n", t_serial);

        // Measure Parallel Time with varying threads
        for (int t = 1; t <= 32; t *= 2) {
            omp_set_num_threads(t);

            t_parallel = omp_get_wtime();
            result = factorial_parallel(N); // Recalculate to measure time
            t_parallel = omp_get_wtime() - t_parallel;

            // Calculate Metrics
            double speedup = t_serial / t_parallel;
            double efficiency = speedup / t;
            double cost = t_parallel * t;

            printf("Threads: %2d | Time: %f | Speedup: %.2f | Efficiency: %.2f | Cost: %.6f\n",
                   t, t_parallel, speedup, efficiency, cost);
        }
    }

    return 0;
}