#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "rank_file"

int main(int argc, char **argv)
{
    int rank, nb_process, read_value;
    MPI_File file;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_process);

    /*
     * Open file read-write
     */
    MPI_File_open(MPI_COMM_WORLD, FILE_PATH, MPI_MODE_CREATE | MPI_MODE_RDWR,
                  MPI_INFO_NULL, &file);

    /*
     * write rank in file
     *
     * Use write_at
     */
    MPI_File_write_at(file, sizeof(int)*rank, &rank, 1, MPI_INT,
                      MPI_STATUS_IGNORE);

    /*
     * read rank in file
     *
     * Use read_at_all
     */
    MPI_File_read_at_all(file, sizeof(int)*rank, &read_value, 1, MPI_INT,
                         MPI_STATUS_IGNORE);
    printf("%d rank reads value %d\n", rank, read_value);

    /*
     * write rank in file by appending
     *
     * Use seek from start + write_all
     */
    MPI_File_seek(file, sizeof(int) * (rank + nb_process), MPI_SEEK_SET);
    MPI_File_write_all(file, &rank, 1, MPI_INT, MPI_STATUS_IGNORE);

    /*
     * read (rank *2) in file
     *
     * Use seek from current + read
     */
    MPI_File_seek(file, -sizeof(int), MPI_SEEK_CUR);
    MPI_File_read(file, &read_value, 1, MPI_INT, MPI_STATUS_IGNORE);
    printf("%d rank reads appended value %d\n", rank, read_value);

    /* close file */
    MPI_File_close(&file);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
