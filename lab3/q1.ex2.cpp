#include<stdio.h>
#include<omp.h>
int main(){
    int a , i;
    int n = 9;
    int b[9];

    #pragma omp parallel shared(a,b) private(i)
    {
    #pragram omp single
        {
            a = 10;
            printf("this single section is executed by thread %d\n", omp_get_thread_num());
        }

    #pragma omp for
             for(i = 0; i < n ; i++)
                 b[i] = a;

    }
    printf(" after the parallel region \n");
    for(i = 0; i < n ; i++)
        printf("b[%d] = %d\n", i , b[i]);  
    return 0;
}