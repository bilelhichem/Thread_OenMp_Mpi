#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <omp.h>

int main(int argc, char **argv)
{
    int start;
    int i;

    start = time(NULL);
    #pragma omp parallel num_threads(2)
    {
        int num_thread = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        #pragma omp master
        {
            printf("First default for loop.\n");
            start = time(NULL);
        }

        #pragma omp for
        for (i = 0; i < num_thread * 4; i++)
        {
            sleep(i);
            printf("Thread %d, loop index %d.\n", thread_id, i);
        }

        #pragma omp master
        {
            printf("First default for loop in %ld s.\n", time(NULL) - start);
            printf("Second static for loop.\n");
            start = time(NULL);
        }

        #pragma omp for schedule(static)
        for (i = 0; i < num_thread * 4; i++)
        {
            sleep(i);
            printf("Thread %d, loop index %d.\n", thread_id, i);
        }

        #pragma omp master
        {
            printf("Second static for loop in %ld s.\n", time(NULL) - start);
            printf("Third (dynamic, 1) for loop.\n");
            start = time(NULL);
        }

        #pragma omp for schedule(dynamic, 1)
        for (i = 0; i < num_thread * 4; i++)
        {
            sleep(i);
            printf("Thread %d, loop index %d.\n", thread_id, i);
        }

        #pragma omp master
        printf("Third (dynamic, 1) for loop in %ld s.\n", time(NULL) - start);
    }

    exit(EXIT_SUCCESS);
}
