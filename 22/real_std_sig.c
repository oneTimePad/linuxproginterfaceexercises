#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "../errors.h"
#define NUM_SIGS 5


int sigs[]={SIGINT,SIGTSTP,SIGQUIT,38,39};


void handler(int sig){
  printf("%s:%d\n","caught",sig);
  return;
}

int
main(int argc,char *argv[]){

  sigset_t blockallsigs;
  sigfillset(&blockallsigs);
  if(sigprocmask(SIG_SETMASK,&blockallsigs,NULL)!=0)
    errnoExit("sigprocmask()");
  int i = 0;
  for(;i<NUM_SIGS;i++){
    struct sigaction action;
    action.sa_handler = &handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if(sigaction(sigs[i],&action,NULL)!=0)
      errnoExit("sigaction()");
  }

  while(1){
    printf("send a signal: type (1..5)->(SIGINT,SIGTSTP,SIGQUIT,SIGRTMIN+5,SIGRTMIN+6)\n-1 to quit\n");
    int sig;
    if(sig ==-1)
      break;
    scanf("%d",&sig);
    if(sig>=6){
      fprintf(stderr,"%s\n","out of bounds index");
      continue;
    }
    kill(getpid(),sigs[sig-1]);
  }
  sigset_t emptysigs;
  sigemptyset(&emptysigs);
  
  if(sigprocmask(SIG_SETMASK,&emptysigs,NULL)!=0)
    errnoExit("sigprocmask()");


  while(1){
    sleep(2);
  }



}
