#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../errors.h"

volatile sig_atomic_t cont =0 ;
void cont_handler(int sig){
  cont = 1;
  return;
}

void tstop_handler(int sig){
  struct sigaction action;
  sigaction(SIGTSTP,NULL,&action); //if we got sigtstp
  action.sa_handler = SIG_DFL;
  sigaction(SIGTSTP,&action,NULL); //suspend
  kill(getpid(),SIGTSTP);
  sleep(2); //SIGTSTP might not happen instantly
  return;
}


int
main(int argc, char *argv[]){

  sigset_t dft_mask,blocked,empty_mask;

  //block SIGCONT and SIGTSTP;
  sigemptyset(&dft_mask);
  sigaddset(&dft_mask,SIGCONT);
  sigaddset(&dft_mask,SIGTSTP);
  if(sigprocmask(SIG_SETMASK,&dft_mask,NULL)!=0)
    errnoExit("sigprocmask()");

  sigemptyset(&empty_mask);

  sigemptyset(&blocked);
  sigaddset(&blocked,SIGCONT);

  struct sigaction cnt_action;
  sigemptyset(&cnt_action.sa_mask);
  cnt_action.sa_flags = 0;
  cnt_action.sa_handler = &cont_handler;
  if(sigaction(SIGCONT,&cnt_action,NULL)!=0)
    errnoExit("sigaction()");

  struct sigaction tstp_action;
  sigemptyset(&tstp_action.sa_mask);
  tstp_action.sa_flags = 0;
  tstp_action.sa_handler = &tstop_handler;
  if(sigaction(SIGTSTP,&tstp_action,NULL)!=0)
    errnoExit("sigaction()");

  sigsuspend(&blocked); //fixes race condition allows us to wait for sigtstp





  //SIGCONT HAPPENED

  int i = 0;
  while(1){ //do some work
    i+=1;
    if(i==20)
      break;
  }

  //unblock all signals
  if(sigprocmask(SIG_SETMASK,&empty_mask,NULL)!=0)
    errnoExit("sigprocmask()");

  if(cont)
    printf("%s\n", "SIGCONT HANDLER WENT OFF!");

  exit(EXIT_SUCCESS);
}
