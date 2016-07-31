#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../errors.h"



int
main(int argc,char *argv[]){

   volatile int fd;
   volatile off_t off;

  if(argc != 3 || strcmp(argv[1],"--help")==0)
    usageExit("%s pathname offset\n",argv[0]);

  fd = open(argv[1],O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if(fd == -1)
    errnoExit("open()");


  off = atoll(argv[2]);

  if(lseek(fd,off,SEEK_SET) == -1)
    errnoExit("lseek()");

  #define TEST_SIZE 4
  if(write(fd,"test",TEST_SIZE) != TEST_SIZE)
    errnoExit("write()");

  if(close(fd) == -1)
    errnoExit("close()");

  exit(EXIT_SUCCESS);




}
