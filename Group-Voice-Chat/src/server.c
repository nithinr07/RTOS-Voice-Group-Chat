// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>

#include "message.h"
#include "util.h"

#define PORT 4141
#define MAX_CONN 1000
#define BUFSIZE 1024

int server_fd;
int client_connections[MAX_CONN];
int client_num;

pthread_t clients[MAX_CONN];
pthread_mutex_t client_num_mutex;

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closing server\n");
		for (int i = 0; i < client_num; i++) {
            pthread_kill(clients[i], SIGKILL);
        }
		for(int i = 0; i < client_num; i++) {
			close(client_connections[i]);
		}
		close(server_fd);
		pthread_exit(NULL);
		exit(0);
	}
}

void *connection_handler(void* clientfd) {
	int client_fd = *((int *) clientfd);
	int valread;
	while(1) {
		// struct Message message;
		uint8_t buf[BUFSIZE];
		// valread = read(client_fd, &message, sizeof(message));
		valread = loop_read(client_fd, buf, sizeof(buf)); 
		for(int i = 0; i < client_num; i++) {
			if(client_fd != client_connections[i]) {
				// loop_write(client_connections[i], &message, sizeof(message));
				loop_write(client_connections[i], buf, sizeof(buf));
			}
		}
	}
}

int main(int argc, char const *argv[]) 
{ 
	int valread, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	signal(SIGINT, close_isr);
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 4141 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
											&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 4141 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 5) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	while(1) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
						(socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 

		pthread_mutex_lock(&client_num_mutex);
		
		client_connections[client_num] = new_socket;
		pthread_create(&clients[client_num], NULL, connection_handler, (void *) &new_socket);
		client_num++; 

		pthread_mutex_unlock(&client_num_mutex);
	}
	return 0; 
}