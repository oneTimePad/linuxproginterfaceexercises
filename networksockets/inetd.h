#ifndef INETD_H
#define INETD_H

#include <sysconf.h>
#ifndef ARG_MAX
#define ARG_MAX 1024
#endif

#define MAX_SERV_NAME 4096
#define MAX_SERV_ARGS 10
typedef enum {NOWAIT, WAIT} inet_flags;

struct service{
	char serv_name[MAX_SERV_NAME+1];
	long serv_sock_type;
	long serv_protocol;
	inet_flags serv_flags;
	char serv_prog[MAX_SERV_NAME];
	char serv_args[MAX_SERV_ARGS * ARG_MAX];

}



#endif 
