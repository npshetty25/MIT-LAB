#include <stdio.h>
#include <string.h>
#include <mpi.h>

int isVowel(char ch)
{
    return (ch=='a'||ch=='e'||ch=='i'||ch=='o'||ch=='u'||
            ch=='A'||ch=='E'||ch=='I'||ch=='O'||ch=='U');
}

int main(int argc, char *argv[])
{
    int rank, size, len;
    char str[100], recvbuf[100];
    int local_count = 0, counts[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter a string: ");
        scanf("%s", str);
        len = strlen(str);
    }

    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int chunk = len / size;

    MPI_Scatter(str, chunk, MPI_CHAR,
                recvbuf, chunk, MPI_CHAR,
                0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk; i++)
    {
        if (!isVowel(recvbuf[i]))
            local_count++;
    }

    MPI_Gather(&local_count, 1, MPI_INT,
               counts, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int total = 0;
        for (int i = 0; i < size; i++)
            total += counts[i];

        printf("Number of non-vowels = %d\n", total);
    }

    MPI_Finalize();
    return 0;
}

