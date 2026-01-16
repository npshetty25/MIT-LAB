/*Write a MPI program using synchronous send. The sender process sends a word to the
receiver. The second process receives the word, toggles each letter of the word and sends
it back to the first process. Both processes use synchronous send operations.*/

#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int rank, size;
    char word[100], modified[100];
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Check if we have at least 2 processes
    if (size < 2) {
        if (rank == 0) {
            fprintf(stderr, "Error: This program requires at least 2 processes.\n");
            fprintf(stderr, "Run with: mpiexec -n 2 ./program_name\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    if (rank == 0) {
        // Master process
        printf("Process 0: Enter a word: ");
        scanf("%s", word);
        printf("Process 0: I entered: %s\n", word);
        
        // Send word to process 1 using SYNCHRONOUS send
        MPI_Ssend(word, strlen(word)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        printf("Process 0: Sent '%s' to Process 1\n", word);
        
        // Receive modified word back from process 1
        MPI_Recv(modified, 100, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
        printf("Process 0: Received modified word: %s\n", modified);
        
    } else if (rank == 1) {
        // Slave process
        // Receive word from process 0
        MPI_Recv(word, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process 1: Received '%s' from Process 0\n", word);
        
        // Toggle each character (lower to upper, upper to lower)
        int len = strlen(word);
        for (int i = 0; i < len; i++) {
            if (islower(word[i]))
                modified[i] = toupper(word[i]);
            else if (isupper(word[i]))
                modified[i] = tolower(word[i]);
            else
                modified[i] = word[i];
        }
        modified[len] = '\0';
        
        printf("Process 1: After toggling: %s\n", modified);
        
        // Send modified word back using SYNCHRONOUS send
        MPI_Ssend(modified, strlen(modified)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        printf("Process 1: Sent modified word back to Process 0\n");
    }
    
    MPI_Finalize();
    return 0;
}
