#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int num_mpi_process;
    int rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello, I'm %d of %d .\n", rank, num_mpi_process);

    MPI_Finalize();

    exit(EXIT_SUCCESS);
}
