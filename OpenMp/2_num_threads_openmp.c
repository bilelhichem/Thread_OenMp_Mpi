#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

int main(int argc, char **argv)
{
    printf("First example : default num_threads or OMP_NUM_THREADS environment "
           "variable.\n");
    #pragma omp parallel
    {
        printf("Hello world : I'm thread %d into %d .\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    printf("Second example : num_threads set by num_threads(3) clause.\n");
    #pragma omp parallel num_threads(3)
    {
        printf("Hello world : I'm thread %d into %d .\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    printf("Third example : num_threads set by omp_set_num_threads(5) "
           "function from omp.h.\n");
    omp_set_num_threads(5);
    #pragma omp parallel
    {
        printf("Hello world : I'm thread %d into %d .\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    printf("Fourth example : without calling omp_set_num_thread.\n");
    #pragma omp parallel
    {
        printf("Hello world : I'm thread %d into %d .\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    printf("Fifth example : with num_threads(7) clause after calling omp_set_num_thread.\n");
    #pragma omp parallel num_threads(7)
    {
        printf("Hello world : I'm thread %d into %d .\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    printf("example 6 : after calling omp_set_num_threads(0).\n");
    omp_set_num_threads(0);
    #pragma omp parallel
    {
        printf("Hello world : I'm thread %d into %d .\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    exit(EXIT_SUCCESS);
}
