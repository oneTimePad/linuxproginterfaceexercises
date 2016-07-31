
#include <fcntl.h>
#include <stdio.h>
#include "mydups.h"


/**
* find lowest fd available and points it to system table entry of oldfd
* oldfd: descriptor to duplicate
* returns: integer fd
**/
int mydup(int oldfd){
    printf("used mydup\n");
    #define LOWEST_FD 0
    int newfd;
    if(fcntl(oldfd,F_GETFL)==-1)
      return -1;

    newfd=fcntl(oldfd,F_DUPFD,LOWEST_FD);
    return newfd;
}

/**
* point newfd to system entry that oldfd points to
* oldfd: fd to duplicate
* newfd: fd to become copy
* return
**/
int mydup2(int oldfd, int newfd){
  printf("used mydup2\n");
  if(fcntl(oldfd,F_GETFL)==-1)
    return -1;

  if(fcntl(newfd,F_GETFL)!=-1)
    if(close(newfd) == -1)
      return -1;

  return (oldfd==newfd) ? newfd: fcntl(oldfd,F_DUPFD,newfd);
}
