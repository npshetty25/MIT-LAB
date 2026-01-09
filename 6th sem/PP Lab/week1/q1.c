//1.Write a simple MPI program to find out pow (x, rank) for all the processes where 'x' is the integer constant and 'rank' is the rank of the process. Write a program in MPI where even ranked process prints "Hello" and odd ranked process prints "World".
#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int x;   // user input

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Take input only from rank 0 */
    if (rank == 0) {
        printf("Enter an integer value for x: ");
        scanf("%d", &x);
    }

    /* Broadcast x to all processes */
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double result = pow(x, rank);

    printf("Process %d: %d^%d = %.0f\n", rank, x, rank, result);

    if (rank % 2 == 0)
        printf("Process %d says: Hello\n", rank);
    else
        printf("Process %d says: World\n", rank);

    MPI_Finalize();
    return 0;
}
