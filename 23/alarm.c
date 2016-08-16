
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

/**
* implements alarm
* seconds: number of seconds for realtime timer
* returns: number of seconds remaing for prev timer or 0 if none
* check for errno
**/
unsigned int my_alarm(unsigned int seconds){
  struct itimerval new_timer;
  struct itimerval cur_timer;
  new_timer.it_interval.tv_sec  = 0;
  new_timer.it_interval.tv_usec = 0;
  new_timer.it_value.tv_usec = 0;
  if(seconds == 0){

    if(getitimer(ITIMER_REAL,&cur_timer)!=0)
      return 0;
    if(cur_timer.it_value.tv_sec == 0 &&cur_timer.it_value.tv_usec ==0 )
      return 0;
    else{
      new_timer.it_value.tv_sec  = 0;
      if(setitimer(ITIMER_REAL,&new_timer,NULL)!=0)
        return 0;
      return 0;
    }
  }
  else{
    new_timer.it_value.tv_sec = seconds;
    if(setitimer(ITIMER_REAL,&new_timer,&cur_timer)!=0)
      return 0;
    return cur_timer.it_value.tv_sec;
  }
}



int
main(int argc, char *argv[]){

  sigset_t  mask;
  sigemptyset(&mask);
  sigaddset(&mask,SIGALRM); //block SIGALRM so it doesn't term proc
  if(sigprocmask(SIG_SETMASK,&mask,NULL)!=0)
    exit(EXIT_FAILURE);
  my_alarm(5); //wait 5 seconds
  siginfo_t info; //wait synchronously for SIGALRM
  sigwaitinfo(&mask,&info);
  if(info.si_signo == SIGALRM) //check signo
    printf("ALARMED!\n");

  exit(EXIT_SUCCESS);
}
