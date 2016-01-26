#include <stdio.h>
#include <string.h>

int main ( int argc , char ** argv )
{
	int socket_serveur;

	if ( argc > 1 && strcmp (argv[1],"-advice") == 0) {
  		printf("Don't Panic ! \n");
  		return 42;
	}

	printf("Need an advice ?\n");

	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_serveur == -1)
	{
		perror("socket_serveur");
		/* traitement de l'erreur */
	}	

	/* Utilisation de la socket serveur */

	return 0;
}
