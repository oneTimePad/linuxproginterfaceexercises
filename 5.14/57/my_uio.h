#ifndef MY_UIO_H
#define MY_UIO_H

#include <sys/types.h>

struct my_iovec{
  void *iov_base;
  size_t iov_len;
};


ssize_t my_readv(int,struct my_iovec*,int);
ssize_t my_writev(int,struct my_iovec*,int);


#endif
