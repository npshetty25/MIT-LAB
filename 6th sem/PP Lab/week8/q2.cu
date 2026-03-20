#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#define N 3   // Fixed size

// (a) Row-wise
__global__ void mat_mult_row_wise(int *a, int *b, int *c) {
    int row = threadIdx.x;

    for (int col = 0; col < N; col++) {
        int sum = 0;
        for (int k = 0; k < N; k++) {
            sum += a[row * N + k] * b[k * N + col];
        }
        c[row * N + col] = sum;
    }
}

// (b) Column-wise
__global__ void mat_mult_col_wise(int *a, int *b, int *c) {
    int col = threadIdx.x;

    for (int row = 0; row < N; row++) {
        int sum = 0;
        for (int k = 0; k < N; k++) {
            sum += a[row * N + k] * b[k * N + col];
        }
        c[row * N + col] = sum;
    }
}

// (c) Element-wise
__global__ void mat_mult_ele_wise(int *a, int *b, int *c) {
    int row = threadIdx.x;
    int col = blockIdx.x;

    int sum = 0;
    for (int k = 0; k < N; k++) {
        sum += a[row * N + k] * b[k * N + col];
    }

    c[row * N + col] = sum;
}

int main() {
    int mat1[N][N], mat2[N][N], res[N][N];
    int *d_a, *d_b, *d_c;

    printf("Enter 3x3 Matrix 1:\n");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &mat1[i][j]);

    printf("Enter 3x3 Matrix 2:\n");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &mat2[i][j]);

    cudaMalloc(&d_a, N * N * sizeof(int));
    cudaMalloc(&d_b, N * N * sizeof(int));
    cudaMalloc(&d_c, N * N * sizeof(int));

    cudaMemcpy(d_a, mat1, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, mat2, N * N * sizeof(int), cudaMemcpyHostToDevice);

    // (a) Row-wise
    printf("\nRow-wise Result:\n");
    mat_mult_row_wise<<<1, N>>>(d_a, d_b, d_c);
    cudaMemcpy(res, d_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", res[i][j]);
        printf("\n");
    }

    // (b) Column-wise
    printf("\nColumn-wise Result:\n");
    mat_mult_col_wise<<<1, N>>>(d_a, d_b, d_c);
    cudaMemcpy(res, d_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", res[i][j]);
        printf("\n");
    }

    // (c) Element-wise
    printf("\nElement-wise Result:\n");
    mat_mult_ele_wise<<<N, N>>>(d_a, d_b, d_c);
    cudaMemcpy(res, d_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", res[i][j]);
        printf("\n");
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
