//3. That takes C program as input, recognizes all the keywords and prints them in upper case.
#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *keywords[] = {
    "auto","break","case","char","continue","do","default","const",
    "double","else","enum","extern","for","if","goto","float","int",
    "long","register","return","signed","static","sizeof","short",
    "struct","switch","typedef","union","void","while","volatile","unsigned"
};

int keywordsLen = 32;

int check(char *word) {
    for (int i = 0; i < keywordsLen; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main() {

    FILE *fptr1, *fptr2;
    char fileName[100];
    char ch, word[100];
    int i = 0;

    printf("Enter filename to open for preprocessing: ");
    scanf("%s", fileName);

    fptr1 = fopen(fileName, "r");
    if (fptr1 == NULL) {
        printf("Could not open file: %s\n", fileName);
        return 1;
    }

    fptr2 = fopen("Q3Out.c", "w");

    ch = fgetc(fptr1);

    while (ch != EOF) {

        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            word[i++] = ch;
            word[i] = '\0';
        }
        else {
            if (i > 0) {
                if (check(word)) {
                    for (int j = 0; word[j]; j++)
                        fputc(toupper(word[j]), fptr2);
                } else {
                    fputs(word, fptr2);
                }
            }
            i = 0;
            word[0] = '\0';
            fputc(ch, fptr2);
        }

        ch = fgetc(fptr1);
    }

    /* Handle last word */
    if (i > 0) {
        if (check(word)) {
            for (int j = 0; word[j]; j++)
                fputc(toupper(word[j]), fptr2);
        } else {
            fputs(word, fptr2);
        }
    }

    fclose(fptr1);
    fclose(fptr2);

    printf("Output file Q3Out.c created\n");
    return 0;
}
