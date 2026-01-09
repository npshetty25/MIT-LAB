//2.Write a program in MPI to simulate simple calculator. Perform each operation using different process in parallel.
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank;
    int a, b;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Take input only from rank 0 */
    if (rank == 0) {
        printf("Enter two integers: ");
        scanf("%d %d", &a, &b);
    }

    /* Broadcast values to all processes */
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Addition: %d + %d = %d\n", a, b, a + b);
    else if (rank == 1)
        printf("Subtraction: %d - %d = %d\n", a, b, a - b);
    else if (rank == 2)
        printf("Multiplication: %d * %d = %d\n", a, b, a * b);
    else if (rank == 3) {
        if (b != 0)
            printf("Division: %d / %d = %d\n", a, b, a / b);
        else
            printf("Division by zero error\n");
    }

    MPI_Finalize();
    return 0;
}
