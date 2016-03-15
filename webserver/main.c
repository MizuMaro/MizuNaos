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
#include "request.h"

#define BUF_SIZE 1024
#define URL_SIZE 255

char *fgets_or_exit(char *buffer, int size, FILE *stream)
{

	if(fgets(buffer, size, stream) == NULL)
	{
		exit(1);
	}
	return buffer;
}

void skip_headers(FILE *client)
{
	char line[BUF_SIZE];

	while(fgets_or_exit(line, BUF_SIZE, client) != NULL && strcmp(line, "\n") != 0 && strcmp(line, "\r\n") != 0);


}

void send_status(FILE *client, int code, const char *reason_phrase)
{
	char res[strlen(reason_phrase)+ 20];
	sprintf(res, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
	fprintf(client, res);
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body)
{

	char res[256+strlen(message_body)];
	sprintf(res, "Connection: close\r\nContent-length: %zu\r\n\r\n%s", strlen(message_body), message_body);
	send_status(client, code, reason_phrase);
	fprintf(client, res);

} 

/*
int check_line(char *line, char *url)
{
	int words = 0;
	char *token;
	token = strtok(line, " ");

	while(token != NULL){
		//On  analyse les mots
		if(words == 0 && !strcmp(token, "GET") == 0)
			return 0;
		if(words == 1 && strlen(token) <= URL_SIZE)
			strcpy(url, token);
		
		if(words == 2 && !(strncmp(token, "HTTP/1.0", 8) == 0 || strncmp(token, "HTTP/1.1", 8) == 0))
			return 0;
		// On récupère mot
		token = strtok(NULL, " ");
		words++;
	}
	return words == 3;
}
*/

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

			//char url[URL_SIZE];

			FILE *client = fdopen(socket_client, "w+");
			//int content_length ;
			char *msg = "<html><head><meta charset=\"UTF-8\"></head><h1>YEA    H!</h1>Bonjour, bienvenue sur le serveur MizuNaos.ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA     ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA     ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA O    RA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA OR    A ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA     ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA     ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA O    RA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA /html>";

			fgets_or_exit(buf,BUF_SIZE,client);
			
			http_request req;

			int bad_request = !parse_http_request(buf,&req);

			if(bad_request)
			{
				send_response(client, 400, "Bad Request", "Bad request\r\n");
			}else if(req.method == HTTP_UNSUPPORTED){
				send_response(client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
			}else if (strcmp(req.url, "/" ) == 0){
				send_response(client, 200, "OK" ,msg);
			}else{
				send_response(client, 404, "Not Found", "Not Found\r\n");
			}

						

			/*
			if(fgets(buf, BUF_SIZE, fdo) != NULL){
				printf("%s\n", buf);
				int check = check_line(buf,url);
				if(check && strcmp(url,"/") == 0){
					msg = "<html><head><meta charset=\"UTF-8\"></head><h1>YEAH!</h1>Bonjour, bienvenue sur le serveur MizuNaos.ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA ORA /html>";
					content_length = strlen(msg);
					printf( "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-length: %d\r\nContent-Type: text/html\r\n\r\n%s", content_length, msg);	
	
					write(socket_client, msg, strlen(msg));
				}else if(check){
					
					msg = "404 Not found : ";
					content_length = strlen(msg);
					printf("HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-length: %d\r\n\r\n%s", content_length, msg);
				}else {
					msg = "400 Bad request";
					content_length = strlen(msg);
					printf( "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length: %d\r\n\r\n%s", content_length, msg);
			
					write(socket_client, msg, strlen(msg));	
				}
				while(strcmp(buf, "\n") != 0 && strcmp(buf, "\r\n") != 0)
					fgets(buf, BUF_SIZE, fdo); 
				*/
				printf("response :\n%d\n", socket_client);
				printf("%d", socket_client);		
			
			fclose(client);
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


