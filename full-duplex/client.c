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
		kill(getpid(), 9)
	}
}

int main(int argc, char const *argv[])
{
	int valread;
	struct sockaddr_in serv_addr;
	char read_buffer[1024] = {0};
	char write_buffer[1024] = {0};
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
	} else {
		printf("Connection Established \n");
	}

	signal(SIGINT, close_isr);

	if(fork() == 0) {
		while(1) {
			memset(write_buffer, 0, sizeof(write_buffer));
			scanf("%[^\n]%*c", write_buffer);
			if(strcmp(write_buffer, "bye") == 0) {
				exit(0);
			}
			send(sock, write_buffer, strlen(write_buffer), 0);
		}	
	} else {
		while(1) {
			memset(read_buffer, 0, sizeof(read_buffer));
			valread = read(sock, read_buffer, 1024); 
			printf("Server : %s\n", read_buffer);
			if(strcmp(read_buffer, "bye") == 0) {
				exit(0);
			}
		}
	}
	return 0;
}