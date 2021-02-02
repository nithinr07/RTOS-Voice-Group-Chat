// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 4141

int sock = 0;

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closing socket\n");
		close(sock);
		exit(0);
	}
}

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char readbuffer[1024] = {0};
	char writebuffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "40.121.60.204", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	signal(SIGINT, close_isr);

	while(1) {
		memset(writebuffer, 0, sizeof(writebuffer));
		printf("Enter Message to server : ");
		scanf("%[^\n]%*c", writebuffer);
		send(sock, writebuffer, strlen(writebuffer), 0);
		memset(readbuffer, 0, sizeof(readbuffer));
		valread = read(sock, readbuffer, 1024); 
		printf("Response from Server : %s\n", readbuffer);
		printf("\n");
	}
	return 0;
}