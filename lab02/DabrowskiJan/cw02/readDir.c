#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat buf;
    long long totalSize = 0;

    // Otwieramy bieżący katalog
    dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    // Przeglądamy zawartość katalogu
    while ((entry = readdir(dir)) != NULL) {
        // Ignorujemy katalogi i specjalne pliki "." i ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Pobieramy statystyki pliku
        if (stat(entry->d_name, &buf) == -1) {
            perror("Error getting file stats");
            closedir(dir);
            return 1;
        }

        // Jeśli plik jest katalogiem, pomijamy go
        if (S_ISDIR(buf.st_mode)) {
            continue;
        }

        // Wypisujemy nazwę i rozmiar pliku
        printf("File: %s, Size: %lld bytes\n", entry->d_name, (long long)buf.st_size);
        totalSize += buf.st_size;
    }

    // Zamykamy katalog
    closedir(dir);

    // Wypisujemy sumaryczny rozmiar wszystkich plików
    printf("Total size of all files: %lld bytes\n", totalSize);

    return 0;
}
