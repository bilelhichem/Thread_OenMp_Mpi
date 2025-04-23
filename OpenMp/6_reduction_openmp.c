#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    int maximum;

    #pragma omp parallel reduction(max:maximum)
    {
        unsigned int rand_seed;
        int thread_id;

        thread_id = omp_get_thread_num();
        rand_seed = time(NULL) + thread_id;
        maximum = rand_r(&rand_seed);
        printf("Me thread %d, I found the local maximum value %d.\n",
               thread_id, maximum);
    }

    printf("The final maximum is %d .\n", maximum);
    exit(EXIT_SUCCESS);
}
