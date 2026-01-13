//1. That takes a file as input and replaces blank spaces and tabs by single space and writes the output to a file
#include <stdio.h>

void main() {

	FILE * fptr1, * fptr2;
	char fileName[100];
	printf("Enter filename to open for preprocessing :");
	scanf("%s", fileName);
	fptr1 = fopen(fileName, "r");
	if (fptr1 != NULL) {
		fptr2 = fopen("Q1Out.txt", "w+");
		char ch = fgetc(fptr1);

		while (ch != EOF) 
		{
			if (ch == '\t' || ch == ' ') 
				{
					fputc(' ', fptr2);
					while (ch != EOF && (ch == '\t' || ch == ' '))
						ch = fgetc(fptr1);
				}
				else
				{
					fputc(ch, fptr2);
					ch = fgetc(fptr1);
				}
			
		}
		printf("Output file Q1Out.txt created\n");
	}
	else
		printf("Could not open file: %s", fileName);
	
}
