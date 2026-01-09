3.Write a program in MPI to toggle the character of a given string indexed by the rank of the process. Hint: Suppose the string is HELLO and there are 5 processes, then process toggle H to h, process I toggle E to 'e and so on.
	#include <mpi.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int rank, size;
    char str[] = "HELLO";

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < 5)
    {
        char ch = str[rank];
        if (isupper(ch))
            ch = tolower(ch);
        else
            ch = toupper(ch);

        printf("Process %d toggled character: %c\n", rank, ch);
    }

    MPI_Finalize();
    return 0;
}
