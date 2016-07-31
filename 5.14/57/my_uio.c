
#include "my_uio.h"


/**
* implements readv
* fd: open fd to read from
* iov: array ptr's to my_iovec structs
* iovcnt: number of my_iovec struct ptrs
* returns: total number bytes read
**/
ssize_t my_readv(int fd,struct my_iovec* iov,int iovcnt){

  ssize_t total_bytes= 0; //total bytes read
  ssize_t bytes_read; //bytes red by one read
  ssize_t bytes_read_for_iov =0; //total bytes put int current iov entry
  volatile void* curr = iov->iov_base; //base ptr for loading bytes
  volatile struct my_iovec* curr_iov = iov; //current iov ptr
  int cnt = 0;
  while((bytes_read=read(fd,curr,curr_iov->iov_len-bytes_read_for_iov))>0){
    bytes_read_for_iov+=bytes_read;
    if(bytes_read_for_iov == curr_iov->iov_len){
      if(cnt+1>=iovcnt){
          total_bytes+=bytes_read;
           break;
      }
      curr_iov = iov+(++cnt);
      curr = curr_iov->iov_base;
      bytes_read_for_iov = 0;
    }
    else
      curr = curr+(bytes_read_for_iov);
    total_bytes+=bytes_read;

  }

  if(bytes_read==-1)
    return -1;

  return total_bytes;


}

/**
* implements my_writev
* fd: fd to write to
* iov: array of ptrs to my_iovec struct
* iovcnt: number of ptr to my_iovec structs
**/
ssize_t my_writev(int fd, struct my_iovec* iov,int iovcnt){
    ssize_t total_bytes =0;
    ssize_t bytes_wrote;
    ssize_t bytes_wrote_for_iov=0;
    volatile void* curr = iov->iov_base;
    volatile struct my_iovec* curr_iov = iov;
    int cnt =0;
    while((bytes_wrote=write(fd,curr,curr_iov->iov_len-bytes_wrote_for_iov))>0){
      bytes_wrote_for_iov+=bytes_wrote;
      if(bytes_wrote_for_iov == curr_iov->iov_len){
        if(cnt+1>=iovcnt){
          total_bytes+=bytes_wrote;
          break;
        }
        curr_iov =(iov+(++cnt));
        curr = curr_iov->iov_base;
        bytes_wrote_for_iov = 0;
      }
      else
        curr = curr+bytes_wrote;
      total_bytes+=bytes_wrote;


    }

      if(bytes_wrote==-1)
        return -1;

      return total_bytes;



}
