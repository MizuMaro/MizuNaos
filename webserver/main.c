#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include "socket.h"
#include <arpa/inet.h>

int main ( int argc , char ** argv )
{
	int socket_serveur = creer_serveur(8080);

	struct sockaddr_in saddr;	

	saddr.sin_family = AF_INET; /* Socket IPv4 */
	saddr.sin_port = htons(8080); /* Port d'écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

	if ( argc > 1 && strcmp (argv[1],"-advice") == 0) {
  		printf("Don't Panic ! \n");
  		return 42;
	}

	printf("Création de la socket_serveur...\n");
	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_serveur == -1)
	{
		perror("socket_serveur");
		/* traitement de l'erreur */
	}
	printf("Socket_serveur crée !\n");	
	/* Utilisation de la socket serveur */

	if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
	{
		perror("bind socket_serveur");
		/* traitement de l'erreur */
	}

	if(listen(socket_serveur, 10) == -1)
	{
		perror("listen socket_serveur");
		/* traitement d'erreur */
	}

	
	int socket_client;

	socket_client = accept(socket_serveur, NULL, NULL);
	if(socket_client == -1)
	{
		perror("accept");
		/* traitement d'erreur */
	}
	/* On peut maintenant dialoguer avec le client */

	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
	write(socket_client, message_bienvenue, strlen(message_bienvenue));


	return socket_serveur;
}
