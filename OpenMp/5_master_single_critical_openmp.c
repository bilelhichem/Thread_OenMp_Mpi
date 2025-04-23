#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NB_ROUND 3
#define MAX_WAIT_SECOND 5

int main(int argc, char **argv)
{
    int count = 0;

    #pragma omp parallel
    {
        unsigned int rand_seed;
        int thread_id;
        int i;

        /* init thread id and random seed */
        thread_id = omp_get_thread_num();
        rand_seed = time(NULL) + thread_id;

        #pragma omp single
        printf("(single) Let's start the game (from %d).\n", thread_id);

        #pragma omp master
        printf("I'm the master, my thread ID is %d and I start.\n", thread_id);

        for (i = 0; i < NB_ROUND; i++) {
            sleep(rand_r(&rand_seed) % MAX_WAIT_SECOND);
            #pragma omp critical
            {
                count = count + 1;
                printf("%d from %d.\n", count, thread_id);
            }
        }

        #pragma omp single
        printf("(single) I finished all my rounds : %d.\n", thread_id);

        #pragma omp master
        printf("Me %d, master, I finished.\n", thread_id);
    }

    exit(EXIT_SUCCESS);
}
