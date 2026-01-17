#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size, n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Take input from user only in master process
        printf("Enter an integer value: ");
        scanf("%d", &n);

        // Send the input value to all other processes
        for (int i = 1; i < size; i++) {
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Receive value from master process
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received value %d\n", rank, n);
    }

    MPI_Finalize();
    return 0;
}

