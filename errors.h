#ifndef _ERRORS_H
#define _ERRORS_H
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef enum { FALSE ,TRUE } _BOOL;

inline void errnoExit(const char* fct_name);
inline  void usageExit(const char* format,...);


#endif
