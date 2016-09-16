
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include "tcp_skt.h"
#include <errno.h>
#include <signal.h>
#include "readline.h"
volatile sig_atomic_t int_signaled = 0;
void int_handler(int sig){
	int_signaled = 1;

}


static int connection_close(int fd){
	errno = 0;
	if(shutdown(fd,SHUT_WR) == -1){
		perror("shutdown");
		errno = 0;
		if(close(fd) == -1){
			perror("close");
			return -1;
		}
		return -1;
	}
	if(close(fd) == -1){
		perror("close");
		return -1;
	}
	return 0;

}



//made this way for simplicity
int communicate(int fd){


	while(1){

		struct rl_type rl; //used for reading in data ending in '\n'
		if(!readLineBufInit(fd,&rl))
			return -1;
		char buffer[DF_BUFFER_SIZE];
		//may act weird, but just for simplicity
		//read all the data the client send up until '\n', if possible, hence simplicity
		errno = 0;
		ssize_t read_in;
		if((read_in=readLineBuf(&rl,buffer,DF_BUFFER_SIZE)) <=0){
			//error occured while reading
			if(read_in == -1){
				printf("%s\n","error on read");
				connection_close(fd);
				return -1;
			}

				

						
			//time to close up, server has been canceled by user
			else if(read_in == -2|| read_in ==0){
				if(int_signaled  ){
					printf("%s\n","service child closing");
					connection_close(fd);
				   	
					return 0;	
				}
				if(read_in == 0){
					printf("%s\n","service child closing");
					if(close(fd) == -1){
						perror("close");
						return -1;
					}
					return 0;
				}
			
								
			}
			
		}
		errno  = 0;
		//send the message the client sent us back to them
		ssize_t write_in;
		if((write_in =write(fd,buffer,read_in)) != read_in){
			if(errno = EINTR){
				if(int_signaled){
					connection_close(fd);
					return 0;
				}
			
			}
			else{
				fprintf(stderr,"%s\n","partial write occured"); //simplified
				connection_close(fd);
				return -1;
			}
		}

		if(int_signaled){
				connection_close(fd);
				return 0;
			
		}

		

	}

}







int main(int argc, char *argv[]){

	struct sigaction int_action;
	sigemptyset(&int_action.sa_mask);
	int_action.sa_flags = 0;
	int_action.sa_handler = int_handler;
	if(sigaction(SIGINT,&int_action,NULL) == -1){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}


	struct addrinfo hints; 		//guides getaddrinfo as to what sockaddr entries to retrieve
	struct addrinfo *result, *rp;   //result: list of addrinfo struct's returned by getaddrinfo
				//rp	: pointer to traverse to addrinfo linked-list returnned by getaddrinfo

	memset(&hints, 0 ,sizeof(struct addrinfo)); //write all zeros to hints
	hints.ai_canonname = NULL; //unused by getaddrinfo
	hints.ai_addr 	   = NULL; //unused by getaddrinfo
	hints.ai_next      = NULL; //unused by getaddrinfo

	hints.ai_family    = AF_INET; //look for IPv4 only
	hints.ai_socktype  = SOCK_STREAM; //look for STREAM sockets
	hints.ai_protocol     = 0;          //use default STREAM sock : TCP <- this line is unnecessary, only for verbosity
	hints.ai_flags 	   = AI_PASSIVE;  //return a socket with wildcard address  0.0.0.0	
	
	//perform the lookup
	if(getaddrinfo(NULL,SERV_PORT,&hints, &result) != 0){
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}

	int optval = 1; //will be used for SOL_REUSEADDR
	//loop through the list of addrinfo struct's given by getaddrinfo
	
	int sfd; //socket file descriptor
	for(rp = result; rp != NULL; rp = rp->ai_next){
		//attemp to create socket with (family: IPv4,IPv6,UNIX,... , socktype: STREAM,DATAGRM,RAW,..., protcol: TCP,UDP, ...)
		sfd = socket(rp -> ai_family, rp -> ai_socktype, rp-> ai_protocol);
		if(sfd == -1) //this socket is "no good"
			continue;

		//ok we found a good one
		
		//sockets have special "socket flags" unlike normal file descriptors
		//since we will be making a passive socket, it's good to set the s-fd reuseaddr 
		if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
			//something terrible went wrong
			perror("setsockopt");
			errno = 0; //reset errno, close might fail too
			if(close(sfd) == -1)
				perror("close");
			freeaddrinfo(result); //getaddrinfo uses dynamic memory, we must free it
			exit(EXIT_FAILURE);
	
		}
		//we need to bind this socket to specifica (host,port) pair so the clients know where to go
		if(bind(sfd, rp->ai_addr,rp->ai_addrlen) == -1){
			perror("bind");
			errno = 0;
			if(close(sfd) == -1)
				perror("close");
			exit(EXIT_FAILURE);
		}

		if(listen(sfd, DF_BACKLOG) == -1){
			perror("listen");
			errno = 0;
			if(close(sfd) == -1)
				perror("close");
			exit(EXIT_FAILURE);
		}
		break;
	}
	
	//loop forever
	while(1) {
		struct sockaddr_in client;
		memset(&client,0,sizeof(struct sockaddr_in));
		int cld;
		socklen_t addrlen = sizeof(struct sockaddr_in);
		if((cld =accept(sfd,(struct sockaddr *)&client,&addrlen)) == -1){

			if(errno != EINTR){
				perror("accept");
				exit(EXIT_FAILURE);
			}
			else{
				if(close(sfd) == -1){
					perror("close");
					exit(EXIT_FAILURE);
				}
				exit(EXIT_SUCCESS);
			}
		}
		switch(fork()){
			case -1:{
				perror("fork");
				exit(EXIT_FAILURE);
				break;
			}
			case 0:{
				if(close(sfd) == -1){
					perror("close");
					exit(EXIT_FAILURE);
				}
				if(communicate(cld) == -1)
					exit(EXIT_FAILURE);
				exit(EXIT_SUCCESS);
				break;
			}
			default:{
				char host[NI_MAXHOST], service[NI_MAXSERV];
				//reverse dns lookup
				if(getnameinfo((struct sockaddr *)&client,addrlen,host,NI_MAXHOST,service,NI_MAXSERV, AI_NUMERICSERV)==0){						printf("%s:(%s:%s)\n","server got connection from",host,service);

				}
				else{
					printf("%s\n","received connection from unknown client");
				}
				if(close(cld) == -1){
					perror("close");
				}
				break;
			}			
				
				
				
				
		}





		if(int_signaled == 1){
		//	if(shutdown(sfd, SHUT_WR) == -1){
		//		perror("shutdown");
		//		errno = 0;
		//		if(close(sfd) == -1)
		//			perror("close");
		//		exit(EXIT_FAILURE);
		//	}
			if(close(sfd) == -1){
				perror("close");
				exit(EXIT_FAILURE);
			}
			printf("%s\n","server exited");
			exit(EXIT_SUCCESS);
		}
	}
	






}
