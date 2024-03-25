#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

extern int copy();
extern int copy_block();


int main() {
    clock_t start = clock();

    copy();

    clock_t end = clock();

    double CopyDuration = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();

    copy_block();

    end = clock();

    double CopyBlockDuration = (double)(end - start) / CLOCKS_PER_SEC;

    FILE *file = fopen("./results/pomiar_zad_2.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "Time of char by char reverse: %f\n", CopyDuration);
    fprintf(file, "Time of block by block reverse: %f\n", CopyBlockDuration);

    fclose(file);

    return 0;
}