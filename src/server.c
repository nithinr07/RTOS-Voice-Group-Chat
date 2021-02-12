// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>

#define PORT 4141
#define MAX 1000

int server_fd;
int client_connections[MAX];
int client_num;

pthread_mutex_t client_num_mutex;

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closing socket\n");
		close(server_fd);
		exit(0);
	}
}

int get_client_id(int clientfd) {
	int id = 0;
	for(int i = 0; i < client_num; i++) {
		if(clientfd == client_connections[i]) {
			id = i;
			break;
		}
	}
	return(id);
}

void *connection_handler(void* clientfd) {
	int client_fd = *((int *) clientfd);
	char buffer[1024] = {0};
	char msg[1024] = {0};
	int valread;
	int id = get_client_id(client_fd);
	while(1) {
		memset(buffer, 0, sizeof(buffer));
		memset(msg, 0, sizeof(msg));
		valread = read(client_fd, buffer, 1024); 
		sprintf(msg, "%d", id);
		strcat(msg," says: ");
		strcat(msg, buffer);
		for(int i = 0; i < client_num; i++) {
			if(client_fd != client_connections[i]) {
				send(client_connections[i], msg, strlen(msg), 0);
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

	pthread_t clients[MAX];

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

	pthread_exit(NULL);
	return 0; 
} 

