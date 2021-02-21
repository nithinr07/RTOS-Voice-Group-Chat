#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>

#include "../Group-Voice-Chat/include/util.h"

#define BUFSIZE 1024

void *read_msg() {
    char name[] = "Nithin";
   /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };

    pa_simple *s = NULL;
    int ret = 1;
    int error;

    /* replace STDIN with the specified file if needed */
        int fd;

        // while ((fd = open("test.wav", O_RDONLY)) < 0) {
        //     // fprintf(stderr, __FILE__": open() failed: %s\n", strerror(errno));
        //     // goto finish;
        // }

        // if (dup2(fd, STDIN_FILENO) < 0) {
        //     fprintf(stderr, __FILE__": dup2() failed: %s\n", strerror(errno));
        //     goto finish;
        // }

        // close(fd);

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }

    for (;;) {
        uint8_t buf[BUFSIZE];
        ssize_t r;

#if 0
        pa_usec_t latency;

        if ((latency = pa_simple_get_latency(s, &error)) == (pa_usec_t) -1) {
            fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        fprintf(stderr, "%0.0f usec    \r", (float)latency);
#endif

        /* Read some data ... */
        if ((r = read(0, buf, sizeof(buf))) <= 0) {
            printf("readinggggggggggggggggggggggggggggggggggggggggggggggg");
            if (r == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            goto finish;
        }

        /* ... and play it */
        // printf("readinggggggggggggggggggggggggggggggggggggggggggggggg");
        if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
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

    // return ret;
} 

void *write_msg() {
	pa_simple *s = NULL;
    char *name = "Nithin : ";
    int ret = 1;
    int error;
    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    int fd = open("test.wav", O_RDWR | O_CREAT, 0777);
    for (;;) {
		struct Message message;
        uint8_t buf[BUFSIZE];
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }
		// memcpy(message.msg, buf, sizeof(buf));
		// strcpy(message.name, name);
        /* And write it to STDOUT */
        if (loop_write(1, buf, sizeof(buf)) != sizeof(buf)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
        }
    }
    ret = 0;
finish:
    if (s)
        pa_simple_free(s);
}

int main() {
    pthread_t read, write;
    pthread_create(&read, NULL, read_msg, NULL);
	pthread_create(&write, NULL, write_msg, NULL);

    pthread_join(read, NULL);
	pthread_join(write, NULL);
}