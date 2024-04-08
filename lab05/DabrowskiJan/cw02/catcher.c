#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>



pid_t senders_signal = 0;
int work_mode = 0;

void handler(int signum, siginfo_t *info, void *context){
    senders_signal = info->si_pid;
    printf("Caught a signal: %d\n",signum);
    work_mode = info->si_value.sival_int;
}

void print_numbers(){
    for(int i=1;i<=100;i++){
        printf("%d\n",i);
    }
}

void print_requests(int requests_no){
    printf("Number of requests: %d\n", requests_no);
}

int main(int argc, char *argv[]){
    if (argc > 1) {
        printf("This program doesn't require arguments\n");
    }

    printf("%d\n", getpid());

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    int requests_no = 0;

    sigset_t emptyset;
    if(sigemptyset(&emptyset) == -1){
        perror("empty set");
        return 1;
    }

    while (work_mode != 3){
        sigsuspend(&emptyset);
        requests_no++;
        switch(work_mode){
            case 1: 
                print_numbers();
                break;
            case 2:
                print_requests(requests_no);
                break;
            default:
                break;
        }

        if (kill(senders_signal, SIGUSR1) == -1) {
            perror("kill");
            return 1;
        }
    }

    return 0;
}

