#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>




int
main(int argc, char *argv[]){


  printf("start\n");
  int err;
  if((err=pthread_join(pthread_self(),NULL)) != 0){
     char *err_str = strerror(err);
      printf("%s\n",err_str);
      _exit(EXIT_FAILURE);
  }
  printf("end\n");







  exit(EXIT_SUCCESS);
}
