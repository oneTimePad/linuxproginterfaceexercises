
bool parseConfig(char *line, struct *service serv){
	int order = 0;
	char *start = line;
	while(*line!='\0'){
		if(*line != ' '){
			start =(start ==NULL) ? line : start;
			line++;
		}
		else{
			switch(order){
				case 0:{
					if(line-start+1 >MAX_SERV_NAME)
						return FALSE;
					strncpy(serv->serv_name,start,line-start+1);
					serv->serv_name[line-start+1] = '\0';
					start = 
					break;
				}
				case 1:{
					if(line-start+1 >       
					       break;
				       }
			}
			start =NULL;
			line++;
		}

	}
	
}


bool readConfig(char *config_file){
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
		if(*(rl->rl_ind_i-1) !='\n'){
			size_t extra_size = rl_buf_size_st - rl_ind_i;
			char large[line+extra_size];
			memcpy(large,line);
			if(readLineBuf(&rl,line+extra_size,extra_size) <0)
				return FALSE;
			line_parse = large;
		}
		if(!parseConfig(line_parse))
			return FALSE;
	}
	


}



volatile sig_atomic_t hup_received = 0;
void hup_handler(int sig){

	hup_received = 1;

}



int main(int argc,char *argv[]){
	struct sigaction hup_action;
	hup_action.flags = SA_RESTART;
	sigemptyset(&hup_action.sa_mask);
	hup_action.handler = hup_handler;
	if(sigaction(SIGHUP,&hup_action,NULL)  == -1){
		errnoExit("sigaction");

	}

	if(daemonize(0) == -1)
		errExit("daemonize");

	openlog(LOG_IDENF,0,LOG_USER);
	readConfig(CONFIG_FILE);

}
