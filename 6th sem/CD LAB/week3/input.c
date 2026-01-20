#include <stdio.h>
#include <stdlib.h>

// This is a single-line comment
/* This is a 
   multi-line comment */

int main() {
    int a, b, c;
    a = 10;
    b = 20;
    c = a + b;
    
    if (a >= 5 && b != 0) {
        c++;
        printf("Result: %d\n", c);
    }
    
    return 0;
}