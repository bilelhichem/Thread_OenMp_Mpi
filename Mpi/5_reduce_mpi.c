#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT_PROCESS 0

int main(int argc, char **argv)
{
    unsigned int random_seed;
    int random_number;
    int rank;
    int max;

    /* init */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    random_seed = time(NULL) + rank;

    /* get a random number */
    random_number = rand_r(&random_seed);
    printf("Rank %d, I get the random number %d .\n", rank, random_number);

    /* reduce to find maximum */
    MPI_Reduce(&random_number, &max, 1, MPI_INT, MPI_MAX, ROOT_PROCESS,
               MPI_COMM_WORLD);

    /* print found max */
    if (rank == ROOT_PROCESS)
        printf("Maximum is %d .\n", max);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
