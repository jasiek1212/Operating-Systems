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

    dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (stat(entry->d_name, &buf) == -1) {
            perror("Error getting file stats");
            closedir(dir);
            return 1;
        }

        if (S_ISDIR(buf.st_mode)) {
            continue;
        }

        printf("File: %s, Size: %lld bytes\n", entry->d_name, (long long)buf.st_size);
        totalSize += buf.st_size;
    }

    closedir(dir);

    printf("Total size of all files: %lld bytes\n", totalSize);

    return 0;
}
