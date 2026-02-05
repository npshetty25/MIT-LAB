//1) Write a MPI program using N processes to find 1! +2! + ..... +N !. Use scan. Also, handle different errors using error handling routines.
#include <stdio.h>
#include <mpi.h>
int factorial(int n) 
{
    int f = 1;
    for(int i = 1; i <= n; i++)
    f *= i;
    return f;
}
int main(int argc, char *argv[])
{
    int rank, size;
    int fact, result;
    int err;
    MPI_Init(&argc, &argv);
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    fact = factorial(rank + 1);
    err = MPI_Reduce(&fact, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(err != MPI_SUCCESS)
    {
        char errstr[MPI_MAX_ERROR_STRING];
        int len;
        MPI_Error_string(err, errstr, &len);
        printf("MPI Error: %s\n", errstr);
    }
    if(rank == 0) 
    {
        printf("Sum of factorials = %d\n", result);
    }
    MPI_Finalize();
    return 0;
}
