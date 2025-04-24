#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT_PROCESS 0

int main(int argc, char **argv)
{
    int *random_number_array = NULL;
    unsigned int random_seed;
    int num_mpi_process;
    int random_number;
    int rank;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    random_seed = time(NULL) + rank;

    /* pick a random number */
    random_number = rand_r(&random_seed);
    printf("Rank %d pick the number %d .\n", rank, random_number);

    /* root process allocate array as result of the gather */
    if (rank == ROOT_PROCESS) {
        random_number_array = malloc(sizeof(int) * num_mpi_process);
        if (!random_number_array) {
            fprintf(stderr, "Unable to allocate random_number_array.\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }
    }

    /* gather all random number to root process */
    MPI_Gather(&random_number, 1, MPI_INT, random_number_array, 1, MPI_INT,
               ROOT_PROCESS, MPI_COMM_WORLD);

    /* root process print all numbers and find the maximum */
    if (rank == ROOT_PROCESS) {
        int max = random_number_array[0];
        unsigned int rank_of_max = 0;
        unsigned int i;


        for (i = 1; i < num_mpi_process; i++) {
            printf("Get %d from rank %d .\n", random_number_array[i], i);
            if (random_number_array[i] > max) {
                max = random_number_array[i];
                rank_of_max = i;
            }
        }

        printf("Max is %d from rank %u .\n", max, rank_of_max);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
