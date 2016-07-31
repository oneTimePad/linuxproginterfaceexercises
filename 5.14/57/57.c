
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include "my_uio.h"
#include "../../errors.h"


#define writev my_writev
#define readv my_readv
typedef struct my_iovec iovec;

#define IOVS 3


int
main(int argc,char* argv[]){


  int fd;
  if((fd=open("testfile",O_CREAT | O_EXCL | O_WRONLY,S_IRUSR | S_IWUSR))==-1){
    if(errno = EEXIST){
      fprintf(stderr,"file exists\n");
      exit(EXIT_FAILURE);
    }
    else
      errnoExit("open()");
  }

  iovec * iov = (iovec*)malloc(sizeof(iovec)*IOVS);

  iov[0].iov_base = malloc(20);
  iov[0].iov_len = 20;
  iov[1].iov_base = malloc(30);
  iov[1].iov_len = 30;
  iov[2].iov_base = malloc(10);
  iov[2].iov_len = 10;

  memset(iov[0].iov_base,0x41,20);
  memset(iov[1].iov_base,0x42,30);
  memset(iov[2].iov_base,0x43,10);

  if(writev(fd,iov,3)==-1)
    errnoExit("writev()");

  memset(iov[0].iov_base,0,20);
  memset(iov[1].iov_base,0,30);
  memset(iov[2].iov_base,0,10);

  if(close(fd)==-1)
    errnoExit("close()");

  if((fd=open("./testfile",O_RDONLY))==-1)
    errnoExit("open()");

  if(readv(fd,iov,3)==-1)
    errnoExit("readv()");

  char first_iov[21];
  snprintf(first_iov,21,"%s",(char*)iov[0].iov_base);
  char second_iov[31];
  snprintf(second_iov,31,"%s",(char*)iov[1].iov_base);
  char third_iov[11];
  snprintf(third_iov,11,"%s",(char*)iov[2].iov_base);

  printf("first: %s\n",first_iov);
  printf("second: %s\n",second_iov);
  printf("third: %s\n",third_iov);

  free(iov[0].iov_base);
  free(iov[1].iov_base);
  free(iov[2].iov_base);
  free(iov);

  if(close(fd)==-1)
    errnoExit("close()");

  exit(EXIT_SUCCESS);
}
