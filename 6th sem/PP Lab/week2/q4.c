#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter an integer value: ");
        scanf("%d", &value);

        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("Final value received back at root (process 0): %d\n", value);
    } 
    else {
        MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        value = value + 1;

        if (rank == size - 1) {
            MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }

        printf("Process %d incremented value to %d and forwarded it\n",
               rank, value);
    }

    MPI_Finalize();
    return 0;
}

