#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include "socket.h"
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <wait.h>

void traitement_signal(int sig)
{
	printf("Signal %d reçu\n", sig);
	int status;
	int pid;
	while((pid = waitpid(-1, &status,WNOHANG)) > 0) {
		printf("Processus numéro %d a retourné %d\n",pid ,status);
	}
}

void initialiser_signaux(void)
{
	struct sigaction sa;
	
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
       	if(sigaction(SIGCHLD, &sa, NULL) == -1)
       	{
                 perror("sigaction(SIGCHLD)");
       	}
}
