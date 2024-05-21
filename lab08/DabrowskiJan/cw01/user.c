#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h> 
#include <wait.h>

#define SHARED_MEMORY_DESCRIPTOR_NAME "/printer_system_shared_memory"
#define MAX_PRINTERS 256
#define MAX_PRINTER_BUFFER_SIZE 256

typedef struct {
    sem_t semaphore;
    char buffer[MAX_PRINTER_BUFFER_SIZE];
    size_t buffer_size;
    int state;
} Printer;

typedef struct {
    Printer printers[MAX_PRINTERS];
    int number_of_printers;
} MemoryMap;

void generate_random_string(char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

void print_user_job(int user_id, MemoryMap* map) {
    char user_buffer[MAX_PRINTER_BUFFER_SIZE];
    generate_random_string(user_buffer, 10);

    int printer_index = -1;
    for (int j = 0; j < map->number_of_printers; j++) {
        int val;
        sem_getvalue(&map->printers[j].semaphore, &val);
        if(val > 0) {
            printer_index = j;
            break;
        }
    }
    if(printer_index == -1)
        printer_index = rand() % map->number_of_printers;

    if(sem_wait(&map->printers[printer_index].semaphore) < 0) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }

    memcpy(map->printers[printer_index].buffer, user_buffer, MAX_PRINTER_BUFFER_SIZE);
    map->printers[printer_index].buffer_size = strlen(user_buffer);

    map->printers[printer_index].state = 1;

    printf("User %d is printing on printer %d\n", user_id, printer_index);
    fflush(stdout);

    sleep(rand() % 3 + 1);
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number of users>\n", argv[0]);
        return -1;
    }

    long num_users = strtol(argv[1], NULL, 10);

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if(memory_fd < 0){
        perror("shm_open");
        return EXIT_FAILURE;
    }

    MemoryMap* map = mmap(NULL, sizeof(MemoryMap), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (map == MAP_FAILED){
        perror("mmap");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_users; i++){
        pid_t user_pid = fork();
        if (user_pid < 0) {
            perror("fork");
            return -1;
        }
        else if(user_pid == 0) {
            while(1) {
                print_user_job(i, map);
            }
            exit(0);
        }
    }

    while(wait(NULL) > 0) {};

    munmap(map, sizeof(MemoryMap));
}
