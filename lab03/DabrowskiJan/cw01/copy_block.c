#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void reverse(char* block, int size);

int copy_block() {
    char block[1024];
    int inputFile, outputFile;
    int byteRead;
    off_t fileSize;

    inputFile = open("./texts/text_copy_block.txt", O_RDONLY);
    outputFile = open("./results/reversed_block.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (inputFile == -1) {
        printf("Error with input file!");
        return 1;
    }

    fileSize = lseek(inputFile, 0, SEEK_END); 
    off_t currentPosition = fileSize; 

    while (currentPosition > 0) {
        off_t offset = currentPosition - sizeof(block);
        if (offset < 0) {
            lseek(inputFile, 0, SEEK_SET);
            read(inputFile, block, currentPosition);
            reverse(block, currentPosition);
            write(outputFile, block, currentPosition);
            break;
        } else {
            lseek(inputFile, offset, SEEK_SET); 
            byteRead = read(inputFile, block, sizeof(block)); 
            reverse(block, byteRead); 
            write(outputFile, block, byteRead); 
            currentPosition -= byteRead; 
        }
    }

    close(inputFile);
    close(outputFile);

    return 0;
}

void reverse(char* block, int size) {
    int i, j;
    char temp;
    for (i = 0, j = size - 1; i < j; i++, j--) {
        temp = block[i];
        block[i] = block[j];
        block[j] = temp;
    }
}

