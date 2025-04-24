#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
 * This program shows the difference between blocking and non-blocking
 * communications.
 *
 * Odd process (4n and 4n+2) send messages to next even process (4n+1 and 4n+3).
 * Between each send/recv, process wait a random number of second.
 *
 * All (4n, 4n+1) process use blocking communication whereas (4n+2, 4n+3)
 * process use non-blocking communication and are simultaneously able to work
 * and wait for communication.
 */

#define NUM_ROUND 4
#define MAX_WAIT_SECOND 3

int main(int argc, char **argv)
{
    unsigned int random_seed;
    int num_mpi_process;
    int random_number;
    int round;
    int rank;

    /* Initialization */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (num_mpi_process < 4 && rank == 0)
        printf("\n\nWARNING : I suggest you run this mpi program with at least "
               "4 tasks.\n\n\n");

    random_seed = time(NULL) + rank;

    /* NUM_ROUND of communication */
    for (round = 0; round < NUM_ROUND; round++) {
        /* 4n send to 4n+1 if it exists by using MPI_Send */
        if (rank % 4 == 0 && rank + 1 < num_mpi_process) {
            sleep(rand_r(&random_seed) % MAX_WAIT_SECOND);
            random_number = rand_r(&random_seed);
            MPI_Send(&random_number, 1, MPI_INT, rank + 1, round,
                     MPI_COMM_WORLD);
            printf("Rank %d, round %d, sent %d .\n", rank, round, random_number);
        }
    
        /* 4n+1 recv from 4n by using MPI_Recv */
        if (rank % 4 == 1) {
            sleep(rand_r(&random_seed) % MAX_WAIT_SECOND);
            MPI_Recv(&random_number, 1, MPI_INT, rank - 1, round, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            printf("Rank %d, round %d, received %d .\n", rank, round,
                   random_number);
        }
    
        /* 4n+2 send to 4n+3 if it exists by using MPI_Isend*/
        if (rank % 4 == 2 && rank + 1 < num_mpi_process) {
            MPI_Request request;
            int flag;

            sleep(rand_r(&random_seed) % MAX_WAIT_SECOND);
            random_number = rand_r(&random_seed);
            MPI_Isend(&random_number, 1, MPI_INT, rank + 1, round,
                      MPI_COMM_WORLD, &request);
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            while (!flag) {
                printf("Rank %d, round %d, I work during my mailing.\n", rank,
                       round);
                usleep(100000); /* 100000us == 1/10 s */
                MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            }

            printf("Rank %d, round %d, sent %d .\n", rank, round, random_number);
        }
    
        /* 4n+3 recv from 4n+2 if it exists by using MPI_Irecv */
        if (rank % 4 == 3) {
            MPI_Request request;
            int flag;

            sleep(rand_r(&random_seed) % MAX_WAIT_SECOND);
            MPI_Irecv(&random_number, 1, MPI_INT, rank - 1, round,
                      MPI_COMM_WORLD, &request);
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            while (!flag) {
                printf("Rank %d, round %d, I work while I'm waiting to receive "
                       "a message.\n", rank, round);
                usleep(100000); /* 100000us == 1/10 s */
                MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            }

            printf("Rank %d, round %d, received %d .\n", rank, round,
                   random_number);
        }
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
