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

int server_fd;
char read_buffer[1024] = {0}; 
char write_buffer[1024] ={0};
int valread, new_socket; 

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closing socket\n");
		close(server_fd);
		exit(0);
	}
}

void *read_msg() {
	while(1) {
		memset(read_buffer, 0, sizeof(read_buffer));
		valread = read(new_socket, read_buffer, 1024); 
		if(valread != 0) {
			printf("Client : %s\n", read_buffer);
		}
	}
} 

void *write_msg() {
	while(1) {
		memset(write_buffer, 0, sizeof(write_buffer));
		scanf("%[^\n]%*c", write_buffer);
		send(new_socket, write_buffer, strlen(write_buffer), 0);
	}
}

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	pthread_t r, w;

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
											&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	signal(SIGINT, close_isr);

	pthread_create(&r, NULL, read_msg, NULL);
	pthread_create(&w, NULL, write_msg, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

	return 0; 
} 

