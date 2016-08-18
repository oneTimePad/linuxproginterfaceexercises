#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>



#define SYNC_SIG SIGUSR1

int
main(int argc, char *argv[]){

  int fd;
  if((fd = open("./test",O_CREAT | O_WRONLY, 444)) ==-1){
    perror("open()");
    exit(EXIT_FAILURE);
  }

  sigset_t blockmask, origmask;
  sigemptyset(&blockmask);
  sigaddset(&blockmask,SYNC_SIG);
  if(sigprocmask(SIG_BLOCK,&blockmask,&origmask)==-1){
    perror("sigprocmask()");
    exit(EXIT_FAILURE);
  }
  setbuf(stdout,NULL);
  setbuf(stderr,NULL);

  pid_t pid;
  switch((pid=vfork())){

    case -1:{
      perror("vfork()");
      exit(EXIT_FAILURE);
      break;
    }
    case 0:{
      printf("child\n");
      if(close(fd)==-1){
        perror("close()");
        _exit(EXIT_FAILURE);
      }
      if(kill(getppid(),SYNC_SIG)==-1){
        perror("kill()");
        _exit(EXIT_FAILURE);
      }
      _exit(EXIT_SUCCESS);
      break;
    }
    default:{
      printf("parent\n");
      siginfo_t info;
      if(sigwaitinfo(&blockmask,&info)==-1){
        perror("sigwaitinfo()");
        exit(EXIT_FAILURE);
      }
      
      if(write(fd,"lolz",4)!=4){
          if(errno != 0)
            perror("write()");
          else{
            fprintf(stderr,"failed to write all data\n");
            exit(EXIT_FAILURE);
          }

      }
      break;
    }

  }


  exit(EXIT_SUCCESS);
}
