#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

void doProcessing(int sock);

int main(int argc, char * argv[])
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in server_address, client_address;
	int result, pid; //replaces 'n' in code, jackass C programmers

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

	return 0;
}

void doProcessing(int sock)
{
	int req, output;
	char buffer[256];
	bzero(buffer, 256);
	char message[1024], response[4096];
	bzero(message,1024);
	bzero(response, 4096);


	req = read(sock, buffer, 255);

	if(req < 0)
	{
		perror("Error reading from socket");
		exit(1);
	}

	printf("Message: %s\n", buffer);
	output = write(sock, "<p>hai</p>", 10);

	if(output < 0)
	{
		perror("Could not write to socket");
		exit(1);
	}
}