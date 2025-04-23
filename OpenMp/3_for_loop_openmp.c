#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#define NB_HELLO 100

int main(int argc, char **argv)
{
    int i;

    #pragma omp parallel for
    for (i = 0; i < NB_HELLO; i++) {
        printf("Hello %d (by thread %d).\n", i, omp_get_thread_num());
    }

    printf("%d end i value\n", i);

    exit(EXIT_FAILURE);
}
