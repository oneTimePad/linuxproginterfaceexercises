
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "../errors.h"




int
main(int argc, char* argv[]){
    if(argc < 2) usageExit("%s [-n num] file\n", argv[0]);
    long long num_lines = 10;
    int c;
    while((c=getopt(argc,argv,"n:"))!=-1){
      if(c == 'n')
        num_lines = atoll(optarg);
    }

    char* file_name = argv[optind];



    int fd;
    if((fd=open(file_name,O_RDONLY)) == -1)
      errnoExit("open()");

    //get the last num_lines bytes
    off_t offset = -1*(num_lines+1);
    if((lseek(fd,offset,SEEK_END)) == -1)
      errnoExit("lseek()");

    off_t len = (long)num_lines;

    //advice to the kernel that we will need to read this segment soon
    errno = 0;
    int error_number;
    if((error_number=posix_fadvise(fd,offset,len,POSIX_FADV_WILLNEED)) != 0){
      switch(error_number){
        case EBADF:
          fprintf(stderr,"posix_fadvise(): not valid fd\n");
          fflush(stdout);
          break;
        case EINVAL:
          fprintf(stderr,"posix_fadvise(): not valid advice\n");
          fflush(stdout);
          break;
        default:
          fprintf(stderr,"posix_fadvise(): got errno %d\n",error_number);
          fflush(stdout);
          break;
      }
      exit(EXIT_FAILURE);
    }

    //read the file
    int bytes_read;
    char file_buffer[num_lines*2]; //allows for more efficient reading
    if((bytes_read=read(fd,file_buffer,num_lines))!=num_lines){
      if(errno!=0)
        errnoExit("read()");
      else{
        fprintf(stderr,"couldn't read all\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
      }
    }

    //write to stdout
    if(write(STDOUT_FILENO,file_buffer,bytes_read)!=num_lines){
      if(errno!=0)
        errnoExit("write()");
      else {
        fprintf(stderr,"couldn't write all\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
      }
    }
    printf("\n");
    fflush(stdout);

    if(close(fd) == -1)
      errnoExit("close()");

    exit(EXIT_SUCCESS);



}
