#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../errors.h"


typedef void (*sighandler_t)(int);




#define SIG_HOLD (void (*) (int))1000
/**
* sets signal disposition
* sig: signal whose disposition to sets
* disp: address of handler, SIG_IGN, SIG_HOLD, or SIG_HOLD
* returns previous disposition or SIG_HOLD, NULL on error
**/
sighandler_t my_sigset(int sig, sighandler_t disp){

  if(disp == SIG_HOLD ){

    sigset_t addedmask,oldmask;
    sigaddset(&addedmask,sig); //add signal to procmask
    if(sigprocmask(SIG_BLOCK,&addedmask,&oldmask)!=0)
      return NULL;
    //check whether sig was a member of the old mask
    switch (sigismember(&oldmask,sig)) {
      case 1:{
        return SIG_HOLD; //if is, return SIG_HOLD
        break;
      }
      case 0:{//if not returns its disposition
        struct sigaction action;
        if(sigaction(sig,NULL,&action)!=0)
          return NULL;
        return action.sa_handler;
        break;
      }
      default:{ //else error
        return NULL;
        break;
      }
    }
  }
  else{ //remove sig to procmask

    sigset_t removemask;
    sigaddset(&removemask,sig);
    if(sigprocmask(SIG_UNBLOCK,&removemask,NULL)!=0)
      return NULL;
  }


  struct sigaction newdisp,olddisp;
  newdisp.sa_handler = (void (*)(int))(disp);
  sigemptyset(&newdisp.sa_mask);
  newdisp.sa_flags = 0; //set disposition and get old one
  if(sigaction(sig,&newdisp,&olddisp)!=0)
    return NULL; //return old
  return olddisp.sa_handler;

}


/**
* add signal to process signal mask
* sig: signal to add
* returns: 0 on success, -1 on error
**/
int my_sighold(int sig){
  sigset_t addmask;
  sigaddset(&addmask,sig);
  if(sigprocmask(SIG_BLOCK,&addmask,NULL)!=0)
    return -1;
  return 0;
}

/**
* removes signal from process signal mask
* sig: signal to remove
* returns: 0 on success, -1 on error
**/
int my_sigrelse(int sig){
  sigset_t removemask;
  sigaddset(&removemask,sig);
  if(sigprocmask(SIG_UNBLOCK,&removemask,NULL)!=0)
    return -1;
  return 0;
}

/**
* sets signal disposition to ignore
* sig: signal to ignore
* returns: 0 on success, -1 on error
**/
int my_sigignore(int sig){
  struct sigaction disp;
  if(sigaction(sig,NULL,&disp)!=0)
    return -1;
  disp.sa_handler = SIG_IGN;
  if(sigaction(sig,&disp,NULL)!=0)
    return -1;
  return 0;
}

/*
* waits until signal is received by remove if from procmask(restores old mask after)
* sig: signal to remove and wait for
* returns: -1 with errno = EINTR(if interrupted), else -1 on error
* EINVAL if signal is not currently blocked
*/
int my_sigpause(int sig){
  sigset_t curmask;
  if(sigprocmask(-1,NULL,&curmask)!=0)
    return -1;
  if(sigismember(&curmask,sig)==0){
    errno = EINVAL;
    return -1;
  }
  sigdelset(&curmask,sig);
  sigsuspend(&curmask);
  errno = EINTR;
  return -1;
}




int
main(int argc, char *argv[]){



  exit(EXIT_SUCCESS);
}
