

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../errors.h"





int
main(int argc, char* argv[]){

    if(argc!=2 || strcmp(argv[1],"--help") == 0)
      usageExit("%s existing_file", argv[0]);

    //file must exist
    if(access(argv[1],F_OK) ==-1)
      errnoExit("access()");

    volatile int fd;

    //open for appending
    if((fd=open(argv[1],O_WRONLY| O_APPEND))==-1)
      errnoExit("open()");

    //seek back to the beggining
    //if(lseek(fd,0,SEEK_SET)==-1)
    //  errnoExit("lseek()");

    //write `test`
    #define TEST_SIZE 4
    if(write(fd,"test",TEST_SIZE)!= TEST_SIZE)
      errnoExit("write()");

    //close so we can start over
    if(close(fd) == -1)
      errnoExit("close()");

    //open for reading
    if((fd =open(argv[1],O_RDONLY)) == -1)
      errnoExit("open()");

    #define BUF_SIZE 512
    char buf[BUF_SIZE];
    ssize_t bytes_read;
    ssize_t cur_bytes_read=0;

    //read in BUF_SIZE-1 bytes, we need room for null
    while((bytes_read=read(fd,buf,BUF_SIZE-1))>0){
        buf[bytes_read] = '\0'; //null since we use strstr
        //starting address of where `test` is
        char* in;
        //if we found test
        if((in=strstr(buf,"test"))!=NULL){
          //print we found it
          char token[5];
          snprintf(token,5,"%s",in);
          printf("found \'%s\'\n",token);
          //compute how many bytes in it is
          printf("test is %ld bytes in\n",(long)cur_bytes_read+(long)(in-((char*)buf)));
          break;
        }
        cur_bytes_read= cur_bytes_read + bytes_read;
    }

    if(close(fd) == -1)
      errnoExit("close()");

    exit(EXIT_SUCCESS);




}
