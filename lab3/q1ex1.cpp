#include<stdio.h>
#include<omp.h>

void funcA();
void funcB();


int main(){
 #pragma omp parallel
    {
    #pragma omp sections{
        #pragma omp section
        {
            funcA();
        }
        #pragma omp section
        {
            funcB();

        }
    } 
}
    return 0;
}

void funcA(){
    printf("This function was executed by thread %d\n" ,omp_get_thread_num());
}

void funcB(){
    printf("This funciton was executed by thread %d\n", omp_get_thread_num())
}