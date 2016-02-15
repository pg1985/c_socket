#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char * argv[])
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in server_address, client_address;
	int result; //replaces 'n' in code, jackass C programmers

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

	newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &clilen);

	if (newsockfd < 0)
	{
		perror("Error on Accepting again");
		exit(1);
	}

	bzero(buffer, 256);
	result = read(newsockfd, buffer, 255);

	if (result < 0)
	{
		perror("Error reading from socket");
		exit(1);
	}

	printf("Message: %s\n", buffer);
	result = write(newsockfd, "I got your message", 18);

	if (result < 0)
	{
		perror("Error writing to socket");
		exit(1);
	}

	return 0;
}