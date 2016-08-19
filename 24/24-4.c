#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>




#define MAX_CHLD 10000


int
main(int argc, char *argv[]){

  int num_chld = 0;
  for(;num_chld<MAX_CHLD;num_chld++){
    pid_t pid;
    switch ((pid=fork())) {
      case -1:{
        perror("fork()");
        exit(EXIT_FAILURE);
      }

      case 0:{
        printf("%d child\n",num_chld);
        _exit(EXIT_SUCCESS);
        break;
      }
      default: {
        printf("%d parent\n",num_chld);
        wait(NULL);
        break;
      }
    }
  }



  exit(EXIT_SUCCESS);
}
