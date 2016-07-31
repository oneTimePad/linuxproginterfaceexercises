#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>
#include "../../errors.h"






int
main(int argc, char* argv[]){


  if((argc!=3 && argc!= 4) || strcmp(argv[1],"--help")==0)
    usageExit("%s filename bytes [x]\n",argv[0]);


    //use O_APPEND
  _BOOL atomic = TRUE;

  //if optional arg was specified
  if(argc==4){
    if(strcmp(argv[3],"x")==0) //is it valid?
      atomic = FALSE;
    else
      usageExit("%s filename bytes [x]",argv[0]);
  }

  int fd;
  mode_t modes = S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR | S_IROTH;

  int flags=O_CREAT | O_WRONLY;
  if(atomic) flags|=O_APPEND;

  if((fd=open(argv[1],flags,modes))==-1)
    errnoExit("open()");

  if(!atomic)
    if(lseek(fd,0,SEEK_END) == -1)
      errnoExit("lseek()");

  char byte_to_write = 0x41;
  ssize_t byte_written;
  long bytes_count;
  long bytes_total = atol(argv[2]);
  //write byte by byte
  while((byte_written=write(fd,&byte_to_write,sizeof(char)))==sizeof(char)&&(++bytes_count)!=bytes_total);


  if(byte_written==-1)
    errnoExit("write()");

  if(close(fd)==-1)
    errnoExit("close()");

  exit(EXIT_SUCCESS);



}
