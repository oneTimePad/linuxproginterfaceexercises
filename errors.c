
#include "errors.h"




/**
* exit and print response to errno
* fct_name: name of fct with error
**/
inline void errnoExit(const char* fct_name){
  if(errno == 0) return;
  perror(fct_name);
  exit(EXIT_FAILURE);
}

/**
* exit and print usage
* format: format string
**/
inline  void usageExit(const char* format,...){
  va_list ap;
  fflush(stderr);
  fprintf(stderr,"Usage: ");
  va_start(ap,format);
  vfprintf(stderr,format,ap);
  va_end(ap);
  fflush(stderr);
  exit(EXIT_FAILURE);
}
