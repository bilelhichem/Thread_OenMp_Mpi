/*
 * User defined reductions were added in OpenMP 4.0 .
 *
 * OpenMP Version 4.0 was released in July 2013.
 */

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct max_with_thread_id {
    int max;
    int thread_id;
};

struct max_with_thread_id update_max_with_thread_id(
        struct max_with_thread_id candidate, struct max_with_thread_id max)
{
    if (candidate.max > max.max)
        return candidate;
    else
        return max;
}

#pragma omp declare reduction(max_with_id : struct max_with_thread_id : \
                              omp_out = update_max_with_thread_id(omp_in, \
                                                                  omp_out)) \
                              initializer(omp_priv = omp_orig)

int main(int argc, char **argv)
{
    struct max_with_thread_id m = {0, 0};


    #pragma omp parallel reduction(max_with_id:x    )
    {
        unsigned int seed;

        m.thread_id = omp_get_thread_num();
        seed = time(NULL) + m.thread_id;
        m.max = rand_r(&seed);
        printf("Local max is %d for thread %d.\n", m.max, m.thread_id);
    }

    printf("Global max is %d from thread %d.\n", m.max, m.thread_id);
    exit(EXIT_SUCCESS);
}
