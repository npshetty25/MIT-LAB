//2. To discard preprocessor directives from the given input ‘C’ file.
#include <stdio.h>

void main()
{
	FILE * fptr1, * fptr2;
	char fileName[100];
	printf("Enter filename to open for preprocessing :");
	scanf("%s", fileName);
	fptr1 = fopen(fileName, "r");
	if (fptr1 != NULL)
    {
		fptr2 = fopen("Q2Out.c", "w+");
		char ch = fgetc(fptr1);

		while (ch != EOF)
        {
			if (ch == '#')
            {
				while (ch != EOF && (ch != '\n'))
					ch = fgetc(fptr1);
				ch = fgetc(fptr1);
			}
			else {
				fputc(ch, fptr2);
				ch = fgetc(fptr1);
			}
		}
		printf("Output file Q2Out.txt created\n");
	}
	else
		printf("Could not open file: %s", fileName);
}
