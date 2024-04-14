#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void sigusr1_handler(int signum) {
    printf("Received SIGUSR1\n");
}

void check_pending(int sig) {
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, sig)) {
        printf("Signal %d is pending.\n", sig);
    } else {
        printf("No pending %d signal.\n", sig);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <none|ignore|handler|mask>\n", argv[0]);
        return 1;
    }

    char *action = argv[1];

    struct sigaction sa;
    sa.sa_flags = 0;

    switch (action[0]) {
        case 'n':
            break;
        case 'i':
            sa.sa_handler = SIG_IGN; 
            sigaction(SIGUSR1, &sa, NULL);
            break;
        case 'h':
            sa.sa_handler = sigusr1_handler; 
            sigaction(SIGUSR1, &sa, NULL);
            break;
        case 'm':
            {
                sigset_t mask;
                sigemptyset(&mask);
                sigaddset(&mask, SIGUSR1);
                sigprocmask(SIG_BLOCK, &mask, NULL); 
            }
            break;
        default:
            printf("Invalid option: %s\n", action);
            return 1;
    }

    kill(getpid(), SIGUSR1);

    if (action[0] == 'm') {
        check_pending(SIGUSR1);
    }

    return 0;
}
