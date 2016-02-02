#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include "socket.h"
#include <arpa/inet.h>

int creer_serveur(int port){
	int socketServeur ;
	socketServeur = socket(AF_INET, SOCK_STREAM, 0);
	if (socketServeur == -1){
		perror("socket serveur");
		return -1;
	}
	//Utilisation de la socket serveur
	struct sockaddr_in socket ;
	socket.sin_family = AF_INET ; // ipv4
	socket.sin_port = htons (port); // port ecoute
	socket.sin_addr.s_addr = INADDR_ANY; 
	if (bind(socketServeur, (struct sockaddr *)&socket, sizeof(socket)) == -1){
		perror("erreur bind");
		return -1;	
	}
	if (listen(socketServeur, 10) == -1){
		perror("erreur listen");
		return -1;
	}	
	return socketServeur;
}
