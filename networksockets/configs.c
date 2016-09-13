#include "configs.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "readline.h"


static bool parseConfig(char *line, struct service *serv){
   	memset(serv,0,sizeof(struct service));
   	int order = 0;
   	char *start = line;
   	while(*line!='\0'){
   		if(*line != ' '){
   			start =(start ==NULL) ? line : start;
   			line++;
   		}
   		else{
   			switch(order++){
   				case 0:{
   					if(line-start+1 >MAX_SERV_NAME)
   						return FALSE;
   					strncpy(serv->serv_name,start,line-start+1);
   					serv->serv_name[line-start+1] = '\0';
   					break;
   				}
   				case 1:{
   					if(line-start+1 >  MAX_SERV_PROT)
   						return FALSE;
   					char tmp_prot[MAX_SERV_PROT+1];
   					strncpy(tmp_prot,start,line-start+1);
   					tmp_prot[line-start+1] = '\0';
   					serv->serv_protocol = (strcmp(tmp_prot,"stream")==0)? SOCK_STREAM : ((strcmp(tmp_prot,"dgram")==0) ? SOCK_DGRAM : -1);
   					if(serv->serv_protocol == -1)
   						return FALSE;	
   					break;
   				}
   				case 2:{
   					if(line-start+1 >MAX_SERV_FLAG)
   						return FALSE;
   					char tmp_flag[MAX_SERV_FLAG+1];
   					strncpy(tmp_flag,start,line-start+1);
   					tmp_flag[line-start+1] = '\0';
   					serv->serv_flag = (strcmp(tmp_flag,"wait")==0) ? WAIT : ((strcmp(tmp_flag,"nowait") ==0) ? WAIT : -INVALID);
   					if(serv->serv_flag == INVALID)
   						return FALSE;			
   					break;
   				}
   				case 3:{
   					if(line-start+1 > MAX_SERV_NAME)
   						return FALSE;
   					strncpy(serv->serv_name,start, line-start+1);
   					serv->serv_name[line-start+1] = '\0';
   					break;
   				}
   				default:{
   					if(line-start+1 >ARG_MAX)
   						return FALSE;
   					if(serv->num_args +1 > MAX_SERV_ARGS)
   						return FALSE;
   					strncpy(serv->serv_args[++serv->num_args],line,line-start+1);
   					serv->serv_args[serv->num_args][line-start+1] = '\0';
   					break;
   				}
   			}
   			start =NULL;
   			line++;
   		}
   
   	}
   	return TRUE;
 	
}


bool readConfig(char *config_file,struct service *servs){
	int cur_serv = 0;
	int fd;
	if( (fd = open(config_file,O_RDONLY)) == -1)
		return FALSE;

	struct rl_type rl;
	if(!readLineBufInit(fd,&rl))
		return FALSE;
	char line[LINE_SIZE];
	char *line_parse;
	while(readLineBuf(&rl,line,LINE_SIZE) > 0){
		line_parse = line;
		if((rl.rl_buffer_p[rl.rl_ind_i-1]) !='\n'){
			int  extra_size = rl.rl_buf_size_st - rl.rl_ind_i;
			char large[LINE_SIZE+extra_size];
			strcpy(large,line);
			if(readLineBuf(&rl,line+extra_size,extra_size) <0)
				return FALSE;
			line_parse = large;
		}
		if(!parseConfig(line_parse,&servs[cur_serv++]))
			return FALSE;
	}
	return TRUE;
	


}


