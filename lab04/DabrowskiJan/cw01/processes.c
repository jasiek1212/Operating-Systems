#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void process(int processes) {
    pid_t child_pid;

    for (int i = 0; i < processes; i++) {
        child_pid = fork();

        if (child_pid == 0) {
            printf("Own: %d, parent: %d\n", (int)getpid(), (int)getppid());
            exit(EXIT_SUCCESS);
        } else if (child_pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    int status;
    for (int i = 0; i < processes; i++) {
        if (wait(&status) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }

    printf("No of processes: %d\n", processes);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int processes = atoi(argv[1]);
    if (processes <= 0) {
        fprintf(stderr, "Number of processes must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    process(processes);

    return 0;
}
