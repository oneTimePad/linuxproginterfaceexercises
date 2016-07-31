#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "../../errors.h"




int
main(int argc, char* argv[]){

  int oldfd;

  if((oldfd = open("./testfile",O_CREAT | O_EXCL, S_IRUSR | S_IWUSR))==-1){
    if(errno == EEXIST){
      fprintf(stderr,"file already exists\n");
      exit(EXIT_FAILURE);
    }
    else
      errnoExit("open()");
  }

  int newfd;
  if((newfd=dup(oldfd))==-1)
      errnoExit("dup()");

  off_t arbitrary_off = 300;
  off_t newfd_offset;
  //set offset 300 from beggining for newfd
  if((newfd_offset=lseek(newfd,arbitrary_off,SEEK_SET))==-1)
    errnoExit("lseek()");
  printf("%d has offset %ld\n",newfd,(long)newfd_offset);

  off_t oldfd_offset;
  //get current offset of oldfd
  if((oldfd_offset=lseek(oldfd,0,SEEK_CUR))==-1)
    errnoExit("lseek()");
  printf("%d has offset %ld\n",oldfd,(long)oldfd_offset);

  if(close(oldfd)==-1)
    errnoExit("close()");

  if(close(newfd)==-1)
    errnoExit("close()");

  exit(EXIT_SUCCESS);

}
