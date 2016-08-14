#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../errors.h"



void int_handler(int sig){
  _exit(0);
}



int
main(int argc, char *argv[]){
  struct sigaction int_action;
  int_action.sa_handler = &int_handler;
  sigemptyset(&int_action.sa_mask);
  int_action.sa_flags = 0;
  if(sigaction(SIGINT,&int_action,NULL)!=0)
    errnoExit("sigaction");

  sigset_t blockmask;
  sigfillset(&blockmask);
  sigdelset(&blockmask,SIGINT);
  if(sigprocmask(SIG_SETMASK,&blockmask,NULL)!=0)
    errnoExit("sigprocmask()");



  sigset_t pending;
  int sig;
  printf("%s","specify signo to ignore:");
  fflush(stdout);
  scanf("%d",&sig);
  fflush(stdin);

  if(sig>=SIGRTMAX || sig<= 0){
    fprintf(stderr,"%s\n","invalid standard signal range");
    exit(1);
  }


  sigpending(&pending);
  if(sigismember(&pending,sig)==0){
    fprintf(stderr,"%s\n","invalid signal, not pending");
    exit(1);
  }
  printf("%s\n","signal will be ignored");
  signal(sig,SIG_IGN);
  sigpending(&pending);
  if(sigismember(&pending,sig)!=1){
    printf("%s %d %s\n","signal:",sig,"is ignored");
    fflush(stdout);
    signal(sig,SIG_DFL);
  }
















}
