#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

char* reverse(char* block, int size);

int copy_block() {
    char block[1024];
    int inputFile, outputFile;
    int byteRead;

    inputFile = open("./texts/text_copy_block.txt", O_RDONLY);
    outputFile = open("./results/reversed_block.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (inputFile == -1) {
        printf("Error with input file!");
        return 1;
    }

    while ((byteRead = read(inputFile, block, sizeof(block))) > 0) {
        char *reversedBlock = reverse(block, byteRead);
        write(outputFile, reversedBlock, byteRead);
        free(reversedBlock); // Free dynamically allocated memory
    }

    close(inputFile);
    close(outputFile);

    return 0;
}

char* reverse(char* block, int size) {
    char* output = (char*)malloc(size); // Allocate memory for the reversed string

    if (output == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        output[i] = block[size - i - 1];
    }

    return output;
}