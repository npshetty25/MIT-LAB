/*
    Original Grammar:
    S → (L) | a
    L → L,S | S

    After removing left recursion:
    S → (L) | a
    L → S L'
    L' → ,S L' | ε
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int curr = 0;
char str[100];

void S();
void L();
void Lprime();

void invalid() {
    printf("Invalid string\n");
    exit(1);
}

void Lprime() {
    if (str[curr] == ',') {
        curr++;
        S();
        Lprime();
    }
    // else ε (epsilon - do nothing)
}

void L() {
    S();
    Lprime();
}

void S() {
    if (str[curr] == '(') {
        curr++;
        L();
        if (str[curr] == ')') {
            curr++;
            return;
        }
        else
            invalid();
    }
    else if (str[curr] == 'a') {
        curr++;
        return;
    }
    else
        invalid();
}

int main() {
    printf("Enter string: ");
    scanf("%s", str);
    strcat(str, "$");
    
    S();
    
    if (str[curr] == '$')
        printf("Valid string\n");
    else
        invalid();
    
    return 0;
}