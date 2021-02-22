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
#include "util.h"

#define PORT 4141
#define BUFSIZE 1024

int sock = 0; int valread;
char write_buffer[BUFSIZE] = {0};
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
	pa_simple *s = NULL;
    int ret = 1;
    int error;
    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__ ": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
	for (;;) {
        uint8_t buf[BUFSIZE];
		struct Message message;
        ssize_t r;
        /* Read some data ... */
        if ((r = read(sock, &message, sizeof(message))) <= 0) {
            if (r == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            goto finish;
        }
		// memcpy(buf, message->msg, sizeof(buf));
		// printf("%s says : \n", message.name);
        /* ... and play it */
        if (pa_simple_write(s, message.msg, sizeof(message.msg), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
        }
    }
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    ret = 0;
finish:
    if (s)
        pa_simple_free(s);

    return NULL;
} 

void *write_msg() {
	pa_simple *s = NULL;
    int ret = 1;
    int error;
    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    for (;;) {
		struct Message message;
        uint8_t buf[BUFSIZE];
        /* Record some data ... */
        if (pa_simple_read(s, message.msg, sizeof(message.msg), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }
		// memcpy(message->msg, buf, sizeof(buf));
		// strcpy(message.name, name);
        /* And write it to STDOUT */
        if (write(sock, &message, sizeof(message)) != sizeof(message)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
        }
    }
    ret = 0;
finish:
    if (s)
        pa_simple_free(s);
}

int main(int argc, char const *argv[])
{
	// printf("Enter name : ");
	// scanf("%[^\n]%*c", name);
	strcpy(name, argv[1]);
	struct sockaddr_in serv_addr;
	// char *ip = "40.121.60.204";
    char ip[32] = "127.0.0.1";
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
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