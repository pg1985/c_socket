#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <regex.h>

void doProcessing(int sock);

regex_t regex;
int reti;

//ghetto way of forcing synchronicity
int busy;

char * opening_tag = "<p>";
char * closing_tag = "</p>";

int main(int argc, char * argv[])
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in server_address, client_address;
	int result, pid; //replaces 'n' in code, jackass C programmers

	//look for BYTE strings.
	reti = regcomp(&regex, "(\/0x)\w+", 0);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror ("Error opening socket");
		exit(1);	
	}

	bzero((char *) &server_address, sizeof(server_address));
	portno = 5001;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
	{
		perror("Error on Binding again");
		exit(1);
	}

	listen(sockfd, 5);
	clilen = sizeof(client_address);

	while(1)
	{

		if(!busy) 
		{
			newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &clilen);

			if (newsockfd < 0)
			{
				perror("Error on Accepting again");
				exit(1);
			}

			pid = fork(); //linux system call to fork a new process.

			if (pid < 0)
			{
				perror("Error forking process");
				exit(1);
			}

			if(pid == 0)  //client process
			{
				close(sockfd);
				doProcessing(newsockfd);
				exit(0);
			}
			else
			{
				close(newsockfd);
			}
		}

		
	}

	return 0;
}

void doProcessing(int sock)
{
	int req, output;
	char buffer[256];
	bzero(buffer, 256);
	char message[1024];
	bzero(message,1024);

	busy = 1;


	req = read(sock, buffer, 1024);

	if(req < 0)
	{
		perror("Error reading from socket");
		exit(1);
	}

	printf("Message: %s\n", buffer);
	char * method = strstr(buffer, "GET");
	printf("req: %d\n", req);

	reti = regexec(&regex, buffer)

	printf("Request Regex Result: %s\n", reti);

	char * response = malloc(strlen(method) + 1024); //

	strcpy(response, opening_tag);
	strcat(response, method);
	strcat(response, closing_tag);

	output = write(sock, response, 1024);

	free(response);
	busy = 0;

	if(output < 0)
	{
		perror("Could not write to socket");
		exit(1);
	}
}