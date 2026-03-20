# include <stdio.h>
#include <cuda_runtime.h>
#define N 3

__global__ void add_row(int *A, int *B, int *C, int cols) {
int row = threadIdx.x;
for (int j = 0; j < cols; j++) {
C[row * cols + j] = A[row * cols + j] + B[row * cols + j];
}
}

__global__ void add_col(int *A, int *B, int *C, int rows) {
int col = threadIdx.x;
for (int i = 0; i < rows; i++) {
C[i * rows + col] = A[i * rows + col] + B[i * rows + col];
}
}

__global__ void add_element(int *A, int *B, int *C) {
int row = threadIdx.y;
int col = threadIdx.x;
int index = row * N + col;
C[index] = A[index] + B[index];
}


int main() {
int A[N][N], B[N][N], C[N][N];
int *d_A, *d_B, *d_C;
int size = N * N * sizeof(int);
 printf("Enter Matrix A:\n");

for(int i=0;i<N;i++)
for(int j=0;j<N;j++)
scanf("%d",&A[i][j]);
printf("Enter Matrix B:\n");
for(int i=0;i<N;i++)
for(int j=0;j<N;j++)
scanf("%d",&B[i][j]);
 cudaMalloc(&d_A, size);
cudaMalloc(&d_B, size);
cudaMalloc(&d_C, size);
//a)
 printf("question A:\n");

 cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
add_row<<<1, N>>>(d_A, d_B, d_C, N);cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
 printf("\nResult Matrix:\n");

for(int i=0;i<N;i++){
for(int j=0;j<N;j++)
printf("%d ", C[i][j]);
printf("\n");
}

//b)

 printf("question B:\n");
 cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
add_col<<<1, N>>>(d_A, d_B, d_C, N);
cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
 printf("\nResult Matrix:\n");
for(int i=0;i<N;i++){
for(int j=0;j<N;j++)
printf("%d ", C[i][j]);
printf("\n");
}

//c)

 printf("question C:\n");
cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
dim3 threads(N, N);
add_element<<<1, threads>>>(d_A, d_B, d_C);
cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
printf("\nResult Matrix:\n");
for(int i=0;i<N;i++){
for(int j=0;j<N;j++)
printf("%d ", C[i][j]);
printf("\n");}
 cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);

return 0;
}
