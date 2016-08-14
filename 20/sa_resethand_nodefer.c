#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../errors.h"

volatile sig_atomic_t handler_execute = 0;

void int_handler(int sig){
  printf("caught\n"); //very very very ... bad

  if(!handler_execute){
    handler_execute = 1;
    printf("send again\n"); //just all bad
    kill(getpid(),SIGINT); //allowed

  }

  return;

}


int
main(int argc, char *argv[]){
  sigset_t blockset,int_mask;
  sigfillset(&blockset);
  if(sigprocmask(SIG_SETMASK,&blockset,NULL)!=0)
    errnoExit("sigprocmask()");

  struct sigaction int_action;
  sigemptyset(&int_action.sa_mask);
  int_action.sa_flags = SA_NODEFER;
  int_action.sa_handler = &int_handler;

  if(sigaction(SIGINT,&int_action,NULL)!=0)
    errnoExit("sigaction()");
  sigfillset(&int_mask);
  sigdelset(&int_mask,SIGINT);

  sigsuspend(&int_mask);


  int_action.sa_flags = SA_RESETHAND;
  if(sigaction(SIGINT,&int_action,NULL)!=0)
    errnoExit("sigaction()");

  sigsuspend(&int_mask);












}
