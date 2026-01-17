#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int *array = NULL;
    int value, result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int buffer_size = size * (sizeof(int) + MPI_BSEND_OVERHEAD);
    void *buffer = malloc(buffer_size);
    MPI_Buffer_attach(buffer, buffer_size);

    if (rank == 0) {
        array = (int *)malloc(size * sizeof(int));

        printf("Enter %d elements:\n", size);
        for (int i = 0; i < size; i++) {
            scanf("%d", &array[i]);
        }

        for (int i = 1; i < size; i++) {
            MPI_Bsend(&array[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (rank % 2 == 0) {
            result = value * value;
            printf("Process %d (Even) received %d, Square = %d\n",
                   rank, value, result);
        } else {
            result = value * value * value;
            printf("Process %d (Odd) received %d, Cube = %d\n",
                   rank, value, result);
        }
    }

    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);

    if (rank == 0) {
        free(array);
    }

    MPI_Finalize();
    return 0;
}

