#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    int num_mpi_process;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* even rank pick a random number and send it to next process if it exists */
    if (!(rank % 2) && rank + 1 < num_mpi_process) {
        unsigned int random_seed = time(NULL) + rank;
        int random_number;

        random_number = rand_r(&random_seed);
        printf("Rank %d, I send the value %d .\n", rank, random_number);
        MPI_Send(&random_number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    /* odd rank receives a number from previous even rank */
    if (rank % 2) {
        int recv_random_number;

        MPI_Recv(&recv_random_number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Rank %d, I received %d .\n", rank, recv_random_number);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
