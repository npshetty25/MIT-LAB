#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>

int fact(int x){
    if(x == 0 || x==1) return 1;
    return x*fact(x-1);
}

int main(int argc, char* argv[]){
    int rank,size;
    int x;
    int array[1024];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if(rank==0){
        printf("enter the array\n");
        for(int i=0;i<size;i++) scanf("%d",&array[i]);
    }
    MPI_Scatter(array, 1, MPI_INT, &x, 1, MPI_INT, 0,MPI_COMM_WORLD);
    x= fact(x);
    printf("computation done by rank %d: %d\n", rank, x);
    MPI_Gather(&x, 1, MPI_INT, array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank==0){
    sleep(0.5);
        int sum=0;
        for(int i=0;i<size;i++) sum+=array[i];

        printf("final sum: %d\n", sum);
    }
    MPI_Finalize();
    return 0;

}
