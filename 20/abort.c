#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../errors.h"



void my_abort(void) {

  kill(getpid(),SIGABRT); //attempt to abort
  struct sigaction action; //if alive, set disposition to DFL
  sigaction(SIGABRT,NULL,&action);
  action.sa_handler = SIG_DFL;
  sigaction(SIGABRT,&action,NULL);
  kill(getpid(),SIGABRT); //now abort
  return;
}

void abrt_handler(int sig){
  printf("%s\n","ABORT CAUGHT!" ); //very very very bad
  return;
}

int
main(int argc,char *argv[]){

  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  action.sa_handler = &abrt_handler;
  if(sigaction(SIGABRT,&action,NULL)!=0)
    errnoExit("sigaction()");

  my_abort();



}
