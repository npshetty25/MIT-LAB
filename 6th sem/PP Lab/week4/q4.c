#include <stdio.h>
#include <string.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
	int rank, size;
	char word[20];
	char out[100] = "", temp[20];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank == 0) 
	{
	printf("Enter word: ");
	scanf("%s", word);
	}
MPI_Bcast(word, 20, MPI_CHAR, 0, MPI_COMM_WORLD);
for(int i = 0; i <= rank; i++)
temp[i] = word[rank];
temp[rank + 1] = '\0';
MPI_Gather(temp, 20, MPI_CHAR,
out, 20, MPI_CHAR, 0, MPI_COMM_WORLD);
if(rank == 0) 
	{
	printf("Output: ");for(int i = 0; i < size; i++)
	printf("%s", &out[i * 20]);
	printf("\n");
	}
MPI_Finalize();
return 0;
}