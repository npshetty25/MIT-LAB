#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void pyramid_str(char * s, int l, char * rs) {
    int i = threadIdx.x;
    int start = (i * (2 * l + ((i - 1) * -1))) / 2; // Sum of AP
    for (int j = 0; j < l - i; j++) {
        rs[start + j] = s[j];
    }
}

int main() {
    char word[100];
    char * d_word, * d_res;
    int wordLen;

    printf("Enter a word ");
    scanf("%s", word);

    wordLen = strlen(word);

    int totalchars = (wordLen * (wordLen + 1)) / 2; // Sum of natural numbers till n

    char * res = (char *) malloc(sizeof(char) * (totalchars + 1));

    cudaMalloc((void **)&d_word, wordLen * sizeof(char));
    cudaMalloc((void **)&d_res, totalchars * sizeof(char));

    cudaMemcpy(d_word, word, wordLen * sizeof(char), cudaMemcpyHostToDevice);

    pyramid_str<<<1, wordLen>>>(d_word, wordLen, d_res);

    cudaMemcpy(res, d_res, totalchars * sizeof(char), cudaMemcpyDeviceToHost);

    res[totalchars] = '\0';

    printf("Final str: %s\n", res);

    cudaFree(d_word);
    cudaFree(d_res);
    return 0;
}