#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int len, chunk;

    char S1[100], S2[100], result[200];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter string S1: ");
        scanf("%s", S1);

        printf("Enter string S2: ");
        scanf("%s", S2);

        len = strlen(S1);
    }

    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    chunk = len / size;

    char localS1[chunk + 1];
    char localS2[chunk + 1];
    char localRes[2 * chunk + 1];

    MPI_Scatter(S1, chunk, MPI_CHAR,
                localS1, chunk, MPI_CHAR,
                0, MPI_COMM_WORLD);

    MPI_Scatter(S2, chunk, MPI_CHAR,
                localS2, chunk, MPI_CHAR,
                0, MPI_COMM_WORLD);

    int k = 0;
    for (int i = 0; i < chunk; i++)
    {
        localRes[k++] = localS1[i];
        localRes[k++] = localS2[i];
    }
    localRes[k] = '\0';

    MPI_Gather(localRes, 2 * chunk, MPI_CHAR,
               result, 2 * chunk, MPI_CHAR,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        result[2 * len] = '\0';
        printf("Resultant String: %s\n", result);
    }

    MPI_Finalize();
    return 0;
}

