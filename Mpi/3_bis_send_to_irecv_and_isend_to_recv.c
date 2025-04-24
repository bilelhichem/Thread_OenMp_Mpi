#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_ROUND 4
#define MAX_WAIT_SECOND 3

int main(int argc, char **argv)
{
    unsigned int random_seed;
    int num_mpi_process;
    int random_number;
    int rank;

    /* Initialization */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    random_seed = time(NULL) + rank;

    /* 2n send to 2n+1 which irecv */
    if (rank % 2 == 0 && rank + 1 < num_mpi_process) {
        random_number = rand_r(&random_seed);
        MPI_Send(&random_number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        printf("Rank %d, send %d .\n", rank, random_number);
    }

    if (rank % 2 == 1) {
        MPI_Request request;

        MPI_Irecv(&random_number, 1, MPI_INT, rank - 1, 0,
                  MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Rank %d, irecv %d .\n", rank, random_number);
    }

    /* 2n isend to 2n+1 which recv */
    if (rank % 2 == 0 && rank + 1 < num_mpi_process) {
        MPI_Request request;

        random_number = rand_r(&random_seed);
        MPI_Isend(&random_number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD,
                  &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Rank %d, isend %d .\n", rank, random_number);
    }

    if (rank % 2 == 1) {
        MPI_Recv(&random_number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Rank %d, recv %d .\n", rank, random_number);
    }
    
    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
