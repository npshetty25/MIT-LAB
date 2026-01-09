//4.Write a program in MPI where even ranked process prints factorial of the rank and odd ranked process prints ranks Fibonacci number
#include <mpi.h>
#include <stdio.h>

long long factorial(int n)
{
    long long fact = 1;
    for (int i = 1; i <= n; i++)
        fact *= i;
    return fact;
}

long long fibonacci(int n)
{
    if (n <= 1) return n;
    long long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main(int argc, char *argv[])
{
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank % 2 == 0)
        printf("Process %d (Even): Factorial = %lld\n", rank, factorial(rank));
    else
        printf("Process %d (Odd): Fibonacci = %lld\n", rank, fibonacci(rank));

    MPI_Finalize();
    return 0;
}
