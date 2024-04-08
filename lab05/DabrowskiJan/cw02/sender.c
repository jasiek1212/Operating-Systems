#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum){
    printf("Signal confirmed.\n");
}

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Usage: %s <catcher_pid> <work_mode>\n", argv[0]);
        return 1;
    }

    pid_t catcher_pid = atoi(argv[1]);
    int work_mode = atoi(argv[2]);

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = handler;
    if(sigaction(SIGUSR1, &sa, NULL) == -1){
        perror("sigaction");
        return 1;
    }

    union sigval sval;
    sval.sival_int = work_mode;

    if(sigqueue(catcher_pid, SIGUSR1, sval) == -1){
        perror("sigqueue");
        return 1;
    }

    sigset_t emptyset;
    if(sigemptyset(&emptyset) == -1){
        perror("empty set");
        return 1;
    }

    sigsuspend(&emptyset);

    return 0;
}