
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../../errors.h"
#include "mydups.h"
#define dup mydup
#define dup2 mydup2
#define MAX_OUT 20


int
main(int argc, char* argv[]){

  int newfd;
  if((newfd = dup(1))==-1)
    errnoExit("dup()");

  char out[MAX_OUT];
  sprintf(out,"wrote to %d\n",newfd);
  if(write(newfd,out,11)!=11)
    errnoExit("write()");

  int newfd2;
  if((newfd2=dup2(newfd,4))==-1)
    errnoExit("dup2()");

  char out2[MAX_OUT];
  sprintf(out2, "wrote to %d\n",newfd2);
  if(write(newfd2,out2,11)!=11)
    errnoExit("write()");

  if(close(newfd)==-1)
    errnoExit("close()");

  if(close(newfd2)==-1)
    errnoExit("close()");



  exit(EXIT_SUCCESS);





}
