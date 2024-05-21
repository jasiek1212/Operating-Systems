#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]){
   pid_t p;
   switch (p=fork()) {
    case -1:{
      perror("fork");
      exit(1);}
    case 0:{  //proces potomny
      sleep(3);
      exit(3);}
    default: {  //proces macierzysty
      int status;
      if  (argc>1) {
       if (atoi(argv[1])==2){
            if(kill(p, SIGINT) == -1){
              perror("kill sigint");
              exit(EXIT_FAILURE);
            }

       }
// Wyślij do potomka sygnał SIGINT

        else if (atoi(argv[1])==9) {
            if(kill(p, SIGKILL)==-1){
              perror("kill sigkill");
              exit(EXIT_FAILURE);
            }
        }

// Wyślij do potomka sygnał SIGKILL

        else printf("Sygnał nieobsłużony\n");
      }
      else printf("Zakończenie przez EXIT\n");
// Czekaj na proces potomny i pobierz jego status zakończenia
//  1) Jeśli proces zakończył się przez exit, wypisz wartość statusu
//  2) Jeśli proces zakończył się sygnałem, wypisz numer sygnału
//  W obu przypadkach powinna zostać zwrócona informacja "Potomek o PID=xxx zakończył się kodem/sygnałem xxx
        int returned;
        if(waitpid(p, &status, 0)==-1){
          perror("waitpid");
          exit(EXIT_FAILURE);
        }
        if(WIFEXITED(status)){
            printf("Wartość statusu: %d\n", WEXITSTATUS(status) );
            returned = WEXITSTATUS(status);
        }
        else if(WIFSIGNALED(status)){
            printf("Numer sygnału: %d\n", WTERMSIG(status));
            returned = WTERMSIG(status);
        }
        printf("Potomek o PID=%d zakończył się kodem/sygnałem %d\n",p,returned);

    }
   }
   return 0;
}
