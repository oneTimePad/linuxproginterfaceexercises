#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>







int
main(int argc, char *argv[]){

  sigset_t blockmask, oldmask;
  sigaddset(&blockmask,SIGRTMIN+5);
  if(sigprocmask(SIG_BLOCK,&blockmask,&oldmask) == -1){
    perror("sigprocmask()");
    exit(EXIT_FAILURE);
  }


  pid_t pid;
  switch ((pid=fork())) {
    case -1:{
      perror("fork()");
      exit(EXIT_FAILURE);
    }
    break;

    case 0:{
      siginfo_t info;
      if(sigwaitinfo(&blockmask,&info) ==-1){
        perror("sigwaitinfo()");
        _exit(EXIT_FAILURE);
      }
      if(info.si_value.sival_int == 100){
        if(kill(getpid(),SIGQUIT) == -1){
          perror("kill()");
          _exit(EXIT_FAILURE);
        }
      }
      else
        _exit(EXIT_FAILURE);
      break;
    }

    default: {
        int i =0;
        int j =0;
        int m = i +j;
        for(;i<j+10;j++)
          m++;
        union sigval val;
        val.sival_int = 100;
        if(sigqueue(pid,SIGRTMIN+5,val) == -1){
          perror("sigqueue()");
          _exit(EXIT_FAILURE);
        }

        break;
    }
  }

  int i = 0;
  int j = 8;
  for(;i<j;i++);



  exit(EXIT_SUCCESS);
}
