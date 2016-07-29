
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>




#define MAX_BUF_SIZE 1024

int main(int argc, char* argv[]){

    if(argc<2){
        fprintf(stderr,"Usage: %s [-a file] \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* fileName = NULL;
    int flags=0;
    int opt;
    while((opt = getopt(argc,argv,"ha:")) !=-1){ //for all options
      switch(opt){
        case 'a':                               //append to existing file
          flags = O_APPEND;
          fileName = optarg;
          break;
        case 'h':                               //respond with help info
          printf("Usage %s [-a file]\n",argv[0]);
          exit(EXIT_SUCCESS);
          break;
        default:
          break;
      }
    }

    flags |= O_CREAT | O_WRONLY;  //write only to file and create 
    mode_t modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP  | S_IROTH | S_IWOTH;

    int fd = -1;
    if((fd=open((fileName==NULL)? argv[0] : fileName,flags,modes))==-1){
      perror("open()");
      exit(EXIT_FAILURE);
    }

    ssize_t bytes_read;
    char buf[MAX_BUF_SIZE];
    while((bytes_read=read(0,buf,MAX_BUF_SIZE))>0){
      ssize_t bytes_write;
      if((bytes_write = write(fd,buf,bytes_read))!=bytes_read){
        perror("write()");
        exit(EXIT_FAILURE);
      }
    }

    if(bytes_read == -1){
      perror("read()");
      exit(EXIT_FAILURE);
    }

    if(close(fd)==-1){
      perror("close()");
      exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);





}
