#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "../errors.h"








int
main(int argc,char *argv[]){

  sigset_t mask;
  sigaddset(&mask,SIGALRM);
  if(sigprocmask(SIG_SETMASK,&mask,NULL)!=0)
    errnoExit("sigprocmask()");
  struct itimerspec timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_nsec = 0;
  timer.it_value.tv_sec = 5;
  timer.it_value.tv_nsec =0;


  //using NULL for sigevent
  timer_t timer1;
  if(timer_create(CLOCK_REALTIME,NULL,&timer1)!=0)
    errnoExit("timer_create()");
  if(timer_settime(&timer1,0,&timer,NULL)!=0)
    errnoExit("timer_settime()");

  siginfo_t info1;
  sigwaitinfo(&mask,&info1);

  if(info1.si_signo == SIGALRM)
    printf("ALARM!\n");
  if(info1.si_value.sival_int == (long)timer1)
    printf("TIMER MATCH!\n");

  //specifying sigevent
  timer_t timer2;
  struct sigevent sev;
  sev.sigev_notify  = SIGEV_SIGNAL;
  sev.sigev_signo   = SIGALRM;
  sev.sigev_value.sival_ptr  = &timer2;
  if(timer_create(CLOCK_REALTIME,&sev,&timer2)!=0)
    errnoExit("timer_create()");
  if(timer_settime(&timer2,9,&timer,NULL)!=0)
    errnoExit("timer_settime()");

  siginfo_t info2;
  sigwaitinfo(&mask,&info2);

  if(info2.si_signo == SIGALRM)
    printf("ALARM!\n");
  if(info2.si_value.sival_int == (long)timer1)
    printf("TIMER MATCH!\n");






  exit(EXIT_SUCCESS);
}
