#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>

int findAVG(int arr[],int x){
    int sum=0;
    for(int i=0;i<x;i++) sum+=arr[i];
    sum/=x;
    return sum;
}
int main(int argc, char* argv[]){
    int rank,size;
    int x, avg;
    int array[1024];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int res[size];
    MPI_Status status;

    if(rank==0){
        printf("enter value of m:");
        scanf("%d", &x);
        printf("enter the array\n");
        for(int i=0;i<x*size;i++) scanf("%d",&array[i]);
    }
    MPI_Bcast(&x,1, MPI_INT, 0, MPI_COMM_WORLD);
    int temp[x];
    MPI_Scatter(array, x, MPI_INT, temp, x, MPI_INT, 0,MPI_COMM_WORLD);
    avg= findAVG(temp,x); 
    printf("avg by rank %d: %d\n", rank, avg);
    MPI_Gather(&avg, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank==0){
    sleep(0.5);
        avg=findAVG(res,size);
        printf("final avg: %d\n", avg);
    }
    MPI_Finalize();
    return 0;

}