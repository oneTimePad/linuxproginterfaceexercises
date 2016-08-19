#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>
#include <signal.h>

#define SIG_ORPH SIGUSR1
#define SIG_SYNC SIGUSR2


int
main(int argc, char *argv[]){

  sigset_t blockmask;
  sigemptyset(&blockmask);
  sigaddset(&blockmask,SIG_ORPH);
  sigaddset(&blockmask,SIG_SYNC);
  if(sigprocmask(SIG_BLOCK,&blockmask,NULL) == -1){
    perror("sigprocmask()");
    exit(EXIT_FAILURE);
  }


  pid_t pid;
  switch((pid = fork())){

    case -1:{
      perror("fork()");
      exit(EXIT_FAILURE);
    }
    case 0:{
        printf("child: %ld, current parent %ld\n",(long)getpid(),(long)getppid());
        siginfo_t info;
        
        if(prctl(PR_SET_PDEATHSIG,SIG_ORPH,0,0,0)==-1){
          perror("prctl()");
          exit(EXIT_FAILURE);
        }


        kill(getppid(),SIG_SYNC);


        if(sigwaitinfo(&blockmask,&info)==-1){
          perror("sigwaitinfo()");
          exit(EXIT_FAILURE);
        }
        if(info.si_signo != SIG_ORPH){
          fprintf(stderr, "%s:%ld\n","bad signal",(long)info.si_signo );
          exit(EXIT_FAILURE);
        }
        else if(info.si_signo == SIG_ORPH){
          printf("orphaned parent id:%ld\n",(long)getppid());
        }
        exit(EXIT_SUCCESS);
        break;
    }
    default:{
      siginfo_t info;
      printf("forked parent: %ld\n",(long)getpid());
      if(sigwaitinfo(&blockmask,&info)==-1){
        perror("sigwaitinfo()");
        exit(EXIT_FAILURE);
      }
      if(info.si_signo != SIG_SYNC){
        fprintf(stderr, "%s:%ld\n","bad signal",(long)info.si_signo );
        exit(EXIT_FAILURE);
      }
      else if(info.si_signo == SIG_SYNC){
        _exit(EXIT_SUCCESS);
      }
      break;
    }


  }


  exit(EXIT_SUCCESS);
}
