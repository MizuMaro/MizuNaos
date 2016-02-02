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

	int socket_client;

	socket_client = accept(socket_serveur, NULL, NULL);
	if(socket_client == -1)
	{
		perror("accept");
		/* traitement d'erreur */
	}
	/* On peut maintenant dialoguer avec le client */

	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
        sleep(1000);
	write(socket_client, message_bienvenue, strlen(message_bienvenue));

	close(socket_client);
	close(socket_serveur);


	return socket_serveur;
}
