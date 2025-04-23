#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

int main(int argc, char **argv)
{
    #pragma omp parallel
    {
        printf("Hello world! %d \n", omp_get_thread_num());
        printf("Hello James! %d \n", omp_get_thread_num());
    }


    exit(EXIT_SUCCESS);
}
