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

#define BUF_SIZE 1024

void initialiser_signaux(void)
{	
	printf("1");
	 if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
         {
                 perror("signal");
         }
	printf("2");
}



int main ()
{

	initialiser_signaux();
	printf("Création de la socket_serveur...\n");
	int socket_serveur = creer_serveur(8080);
	if(socket_serveur == -1)
	{
		perror("socket_serveur");
		return -1;
		/* traitement de l'erreur */
	}
	printf("Socket_serveur crée !\n");	
	/* Utilisation de la socket serveur */

	int socket_client;


	while(1){
	socket_client = accept(socket_serveur, NULL, NULL);
	if(socket_client == -1)
	{
		perror("accept");
		/* traitement d'erreur */
		return -1;
	}

	
	if(fork() == 0){
		/* On peut maintenant dialoguer avec le client */
	 	sleep(1);
		const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";

		write(socket_client, message_bienvenue, strlen(message_bienvenue));

	
		char buf[BUF_SIZE];
		bzero(buf, BUF_SIZE);

		int n;
		while((n = read(socket_client, buf, BUF_SIZE-1)) > 0){ 
			if(n == -1){
				perror("read socket");
				return 1;	
			}

			buf[BUF_SIZE-1] = '\0';
			printf("%s", buf);		
			if(write(socket_client, buf, n) <= 0)
				break;

			bzero(buf, BUF_SIZE);
				
			}
		}else{
		
				close(socket_client);
				exit(1);
			}

	}

	close(socket_client);
	close(socket_serveur);


	return socket_serveur;
}

