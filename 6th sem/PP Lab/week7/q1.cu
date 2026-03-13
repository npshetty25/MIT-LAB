#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void count_occ_of_word(char * str, char * word, int * space_indices, unsigned int *d_count) {
    int i = threadIdx.x;
    int start, end;
    if (i == 0) {
        start = 0;
        end = space_indices[0] - 1;
    }
    else {
        start = space_indices[i - 1] + 1;
        end = space_indices[i] - 1;
    }

    int flag = 1, ind = 0;
    for (int j = start; j <= end; j++) {
        if (word[ind++] != str[j]) {
            flag = 0;
            break;
        }
    }
    if (flag)
        atomicAdd(d_count, 1);
}

int main() {
    char str[100], word[100];
    char * d_str, * d_word;
    int * d_space_indices;
    unsigned int count = 0, *d_count, result;

    printf("Enter a string ");
    scanf("%[^\n]s", str);
    fflush(stdin);
    printf("Enter the word ");
    scanf("%s", word);

    int i = 0, space_count = 0, *space_indices = (int *) malloc(sizeof(int));
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            space_count++;
            space_indices = (int *) realloc(space_indices, space_count * sizeof(int));
            space_indices[space_count - 1] = i;
        }
        i++;
    }
    space_count++;
    space_indices = (int *) realloc(space_indices, space_count * sizeof(int));
    space_indices[space_count - 1] = strlen(str);

    cudaMalloc((void **)&d_str, strlen(str) * sizeof(char));
    cudaMalloc((void **)&d_word, strlen(word) * sizeof(char));
    cudaMalloc((void **)&d_space_indices, space_count * sizeof(int));
    cudaMalloc((void **)&d_count, sizeof(unsigned int));

    cudaMemcpy(d_str, str, strlen(str) * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_word, word, strlen(word) * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_space_indices, space_indices, space_count * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_count, &count, sizeof(unsigned int), cudaMemcpyHostToDevice);

    count_occ_of_word<<<1, space_count>>>(d_str, d_word, d_space_indices, d_count);

    cudaMemcpy(&result, d_count, sizeof(unsigned int), cudaMemcpyDeviceToHost);

    printf("Total occurences of word = %u\n", result);

    cudaFree(d_str);
    cudaFree(d_word);
    cudaFree(d_space_indices);
    cudaFree(d_count);
    return 0;
}