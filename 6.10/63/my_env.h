#ifndef MY_ENV_H
#define MY_ENV_H
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//max length varies too much on each system, just choose something small
//warning: but might result in overflowing total env size
#define MAX_ENV_NAME 2048
#define MAX_VAL_NAME 2048
#define MALLOC_MAGIC_STRING "AGHI"
#define MAGIC_STRING_LEN 4

extern char** environ;


int my_setenv(const char *,const char *, int);
int my_unsetenv(const char *);


#endif
