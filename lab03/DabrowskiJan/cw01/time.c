#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

extern int copy();
extern int copy_block();


int main() {
    // Pobieramy czas początkowy
    clock_t start = clock();

    // Wywołujemy funkcję do przetwarzania katalogu
    copy();

    // Pobieramy czas końcowy
    clock_t end = clock();

    // Obliczamy czas trwania w sekundach
    double CopyDuration = (double)(end - start) / CLOCKS_PER_SEC;

    // Pobieramy czas początkowy
    start = clock();

    copy_block();

    // Pobieramy czas końcowy
    end = clock();

    // Obliczamy czas trwania w sekundach
    double CopyBlockDuration = (double)(end - start) / CLOCKS_PER_SEC;

    // Otwieramy plik do zapisu
    FILE *file = fopen("./results/pomiar_zad_2.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Zapisujemy wyniki do pliku
    fprintf(file, "Time of char by char reverse: %f\n", CopyDuration);
    fprintf(file, "Time of block by block reverse: %f\n", CopyBlockDuration);

    // Zamykamy plik
    fclose(file);

    return 0;
}