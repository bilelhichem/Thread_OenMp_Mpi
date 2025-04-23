/*
 * User defined reductions were added in OpenMP 4.0 .
 *
 * OpenMP Version 4.0 was released in July 2013.
 */

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int thread_id;
    int i;


    #pragma omp parallel
    {
        #pragma omp for lastprivate(thread_id)
        for (i = 0; i < omp_get_num_threads(); i++) {
            unsigned int seed;

            thread_id = omp_get_thread_num();
            sleep(thread_id);
            printf("Local thread_id %d into for.\n", thread_id);
        }

        printf("Local thread_id %d after for.\n", thread_id);
    }

    printf("Final thread_id %d .\n", thread_id);
    exit(EXIT_SUCCESS);
}
