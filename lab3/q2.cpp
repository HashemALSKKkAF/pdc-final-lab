#include<stidio.h>
#include<omp.h>

int main(int agrc , char** argv){
    printf("this is main thread.\n");
    omp_set_dynamic(0);
    int id;

    #pragma omp parallel private(id) num_threads(6)
    {
        id = omp_get_thread_num();

    #pragma omp single
    {
        printf("the total number of threads is %d\n", omp_get_num_threads());
        printf("this single section is executed by thread %d\n", id);
    }    
    #pragma omp sections
    {
        #pragma omp section
        {
            printf("this section A is executed by thread %d\n", id);
        }
        #pragma omp section
        {
            printf("this section B is executed by thread %d\n", id);
        }
        #pragma omp section
        {
            printf("this section C is executed by thread %d\n", id);
        }
    }
}
    printf("this is block for the main thread.\n");
    printf("Goodbye from the main thread.\n");
    return 0;

}