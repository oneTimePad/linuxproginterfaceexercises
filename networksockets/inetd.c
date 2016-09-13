  

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
