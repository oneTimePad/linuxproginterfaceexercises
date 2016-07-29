#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>


#define MAX_BUF_SIZE 1024


typedef enum { FALSE, TRUE} _BOOL;

static _BOOL readAndWriteAll(int src,int dst, size_t total_amt){
  #define MAX_BUF_SIZE 1024

  int bytes_read;
  char buf[MAX_BUF_SIZE];
  while((bytes_read=read(src,buf,MAX_BUF_SIZE))>0){
    int bytes_write;
    if((bytes_write=write(dst,buf,bytes_read))!=bytes_read){
      perror("write()");
      return FALSE;
    }
  }

  if(bytes_read==-1){
    perror("read()");
    return FALSE;
  }
  return TRUE;
}

static void errErrnoExit(const char* fct){
  perror(fct);
  exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]){

  if(argc<3){
    fprintf(stderr,"Usage: %s src dst\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  int opt;
  while((opt=getopt(argc,argv,"h"))>0){
    switch(opt){
      case 'h':
        printf("Usage: %s src dst\n", argv[0]);
        exit(EXIT_SUCCESS);
        break;
      default:
        break;
    }
  }

  mode_t modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP  | S_IROTH | S_IWOTH;

  int src;
  if((src=open(argv[1],O_RDWR))==-1){
    perror("open()");
    exit(EXIT_FAILURE);
  }

  if(lseek(src,50,SEEK_SET)==(off_t)-1){
    perror("lseek()");
    exit(EXIT_FAILURE);
  }

  if(write(src,"FFF",4)==-1){
    perror("write()");
    exit(EXIT_FAILURE);
  }

  if(lseek(src,0,SEEK_SET)==(off_t)-1){
    perror("lseek()");
    exit(EXIT_FAILURE);
  }


  int dst;
  if((dst=open(argv[2],O_CREAT | O_WRONLY, modes))==-1){
    perror("open()");
    exit(EXIT_FAILURE);
  }


  //retrieve the offset to the end of the file
  struct stat src_stat;
  if(stat(argv[1],&src_stat)==-1){
    perror("stat()");
    exit(EXIT_FAILURE);
  }
  off_t end_of_file = src_stat.st_size;


  off_t previous = 0;
  off_t current=0;

  while((current=lseek(src,previous,SEEK_HOLE))!=(off_t)-1){ // look for the next hole

      if(previous!=current){  //when previous is not current, previous points to data
        if(lseek(src,previous,SEEK_SET)==-1) // go back to the data offset
          errErrnoExit("lseek()");

        if(!readAndWriteAll(src,dst,current-previous)) //write the data, it ends at current (hole)
          exit(EXIT_FAILURE);

        if(lseek(src,current,SEEK_SET)==-1) //go back to beggining of the hole
          errErrnoExit("lseek()");

        if(current==end_of_file)  //if the beggining of the hole is at the end of file...we are done
          break;
      }

      else{ // previous and current are equal
        if((current=lseek(src,previous,SEEK_DATA))==(off_t)-1) //look for next data offset
          errErrnoExit("lseek()");

        if(lseek(dst,current-previous,SEEK_CUR)==(off_t)-1)  //put the holes in the dst
          errErrnoExit("lseek()");

      }

      previous = current; //set previous to current

  }

  if(current==-1){
    errErrnoExit("lseek()");
  }



  if(close(src)==-1){
    perror("close()");
    exit(EXIT_FAILURE);
  }

  if(close(dst)==-1){
    perror("close");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);



}
