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



int check_line(char *line){
int words = 0;
	char *token;
	token = strtok(line, " ");

	while(token != NULL){
		//On va maintenant analyser les mots
		if(words == 0 && !strcmp(token, "GET") == 0)
			return 0;
		if(words == 2 && !(strncmp(token, "HTTP/1.0", 8) == 0 || strncmp(token, "HTTP/1.1", 8) == 0))
			return 0;
		// On récupère le prochain mot
		token = strtok(NULL, " ");
		words++;
	}
	return words == 3;
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
				
			char buf[BUF_SIZE];
			bzero(buf, BUF_SIZE);

			FILE *fdo = fdopen(socket_client, "w+");
			int content_length ;
			char *msg;
			
			if(fgets(buf, BUF_SIZE, fdo) != NULL){
				printf("%s\n", buf);
			if(check_line(buf)){
				msg = "<html><head><meta charset=\"UTF-8\"></head><h1>YEAH!</h1>Bonjour, bienvenue sur le serveur MizuNaos.ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA /html>";
				content_length = strlen(msg);
				printf( "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-length: %d\r\nContent-Type: text/html\r\n\r\n%s", content_length, msg);	
	
				write(socket_client, msg, strlen(msg));
			}else{
				msg = "400 Bad request";
				content_length = strlen(msg);
				printf( "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length: %d\r\n\r\n%s", content_length, msg);
			
				write(socket_client, msg, strlen(msg));	
			}
			while(strcmp(buf, "\n") != 0 && strcmp(buf, "\r\n") != 0)
				fgets(buf, BUF_SIZE, fdo); 

			printf("response :\n%d\n", socket_client);
			printf("%d", socket_client);		
		}
			fclose(fdo);
			perror("socket closed");
			close(socket_client);
			exit(1);
		}else{
		
			close(socket_client);
		}

	}

	close(socket_serveur);


	return socket_serveur;
}


