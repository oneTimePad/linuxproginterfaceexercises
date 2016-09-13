#ifndef CONFIGS_H
#define CONFIGS_H

#include <unistd.h>
#include "bool.h"
#ifndef ARG_MAX
#define ARG_MAX 1024
#endif

#define MAX_SERV_NAME 4096
#define MAX_SERV_ARGS 10
#define MAX_SERV_PROT 10
#define MAX_SERV_FLAG 10

#define LINE_SIZE     4096

typedef enum {INVALID,NOWAIT, WAIT} inet_flags;

struct service{
	char serv_name[MAX_SERV_NAME+1];
	long serv_sock_typ;
	long serv_protocol;
	inet_flags serv_flag;
	char serv_prog[MAX_SERV_NAME];
	char serv_args[MAX_SERV_ARGS][ARG_MAX];
	int num_args;
};



#endif
