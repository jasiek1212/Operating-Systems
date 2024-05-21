#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <semaphore.h> 

#define SHARED_MEMORY_DESCRIPTOR_NAME "/printer_system_shared_memory" 
#define MAX_PRINTERS 256
#define MAX_PRINTER_BUFFER_SIZE 256

typedef struct {
    sem_t semaphore;
    char buffer[MAX_PRINTER_BUFFER_SIZE];
    size_t buffer_size;
    int state; // 1 when printing, 0 when available
} Printer;

typedef struct {
    Printer printers[MAX_PRINTERS];
    int number_of_printers;
} MemoryMap;

void print_job(Printer *printer) {
    while(1){
        if (printer->state == 1) {
            for (int j = 0; j < printer->buffer_size; j++) {
                printf("%c", printer->buffer[j]);
                sleep(1);
            }
            printf("\n");
            fflush(stdout);
            printer->state = 0;
            sem_post(&printer->semaphore);
        }  
    }
}

void initialize_memory_map(MemoryMap* memory_map, long num_printers) {
    memset(memory_map, 0, sizeof(MemoryMap));
    memory_map->number_of_printers = num_printers;
}

void initialize_printers(MemoryMap* memory_map) {
    for (int i = 0; i < memory_map->number_of_printers; i++){
        sem_init(&memory_map->printers[i].semaphore, 1, 1);
    }
}

void cleanup_printers(MemoryMap* memory_map) {
    for (int i = 0; i < memory_map->number_of_printers; i++) {
        sem_destroy(&memory_map->printers[i].semaphore);
    }
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number of printers>\n", argv[0]);
        return EXIT_FAILURE;
    } 

    long num_printers = strtol(argv[1], NULL, 10);

    if (num_printers > MAX_PRINTERS) {
        printf("Number of printers exceeds the maximum allowed limit. Limiting to max...\n");
        num_printers = MAX_PRINTERS;
    }

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0){
        perror("Failed to open shared memory descriptor");
        return EXIT_FAILURE;
    }

    if(ftruncate(memory_fd, sizeof(MemoryMap)) < 0){
        perror("Failed to set shared memory size");
        return EXIT_FAILURE;
    }

    MemoryMap* memory_map = mmap(NULL, sizeof(MemoryMap), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED){
        perror("Failed to map shared memory");
        return EXIT_FAILURE;
    }

    initialize_memory_map(memory_map, num_printers);
    initialize_printers(memory_map);

    for (int i = 0; i < num_printers; i++){
        pid_t printer_pid = fork();
        if(printer_pid < 0) {
            perror("Failed to fork process for printer");
            return EXIT_FAILURE;
        }
        else if(printer_pid == 0) {
            print_job(&memory_map->printers[i]);
            exit(EXIT_SUCCESS);
        }
    }

    while(wait(NULL) > 0) {}

    cleanup_printers(memory_map);

    munmap(memory_map, sizeof(MemoryMap));
    shm_unlink(SHARED_MEMORY_DESCRIPTOR_NAME);

    return EXIT_SUCCESS;
}
