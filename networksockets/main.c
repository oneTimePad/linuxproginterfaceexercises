#include <stdlib.h>
#include "readline.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>




int
main(int argc, char *argv){


	int fd;
	if((fd = open("test",O_RDONLY)) == -1){
		perror("open()");
		exit(EXIT_FAILURE);
	}
	struct rl_type rl;
	if(!readLineBufInit(fd,&rl)){
		perror("readLineBufInit()");
		exit(EXIT_FAILURE);
	}
	char line[8012];
	if(readLineBuf(&rl,line,10) <=0){
		fprintf(stderr,"error");
		exit(EXIT_FAILURE);
	}
	printf("%s\n",line);
	if(readLineBuf(&rl,line,10) <=0){
		fprintf(stderr,"error");
		exit(EXIT_FAILURE);
	}
	printf("%s\n",line);
	if(readLineBuf(&rl,line,10) <=0){
		fprintf(stderr,"error");
		exit(EXIT_FAILURE);
	}
	printf("%s\n",line);	
	exit(EXIT_SUCCESS);
}
