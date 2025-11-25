#include <stdio.h>
#include <omp.h>
int main (){
    omp_set_dynamic(0); 
    omp_set_num_threads(16);


    #progma omp parallel num_threads(10)
    {
        printf("Threads in parallel region %d\n", omp_get_num_threads());
    }   printf("the parallel region is executed by thread %d\n", omp_get_thread_num());
     // Set the number of threads to 4
}