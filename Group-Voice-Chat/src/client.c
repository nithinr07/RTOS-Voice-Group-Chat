// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "message.h"

#define PORT 4141

int sock = 0; int valread;
char read_buffer[1024] = {0};
char write_buffer[1024] = {0};
char name[100];

pthread_t r, w;

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closing socket\n");
		pthread_kill(r, SIGKILL);
		pthread_kill(w, SIGKILL);
		close(sock);
		exit(0);
	}
}

void *read_msg() {
	while(1) {
		struct Message message;
		valread = read(sock, &message, sizeof(message)); 
		if(valread != 0) {
			fflush(stdout);
			printf("%s : %s\n", message.name, message.msg);
		}
	}
} 

void *write_msg() {
	while(1) {
		struct Message message;
		strcpy(message.name, name);
		memset(write_buffer, 0, sizeof(write_buffer));
		scanf("%[^\n]%*c", write_buffer);
		strcpy(message.msg, write_buffer);
		send(sock, &message, sizeof(message), 0);
	}
}

int main(int argc, char const *argv[])
{
	printf("Enter name : ");
	scanf("%[^\n]%*c", name);
	struct sockaddr_in serv_addr;
	
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

	pthread_create(&r, NULL, read_msg, NULL);
	pthread_create(&w, NULL, write_msg, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

	return 0;
}