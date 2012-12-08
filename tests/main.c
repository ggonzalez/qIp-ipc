#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "qip-ipc.h"

#define THR1_PORT	3333
#define LOCAL_NODE	"127.0.0.1"

/*
 * Testing communication and data availability checking
 */
void *
thread1(void *notused)
{
	qip_peer_t	*peer = NULL;
	char		*buff = NULL;
	int		fd = 0;

	if ((fd = qip_new_server(THR1_PORT)) < 0) {
		fprintf(stderr, "ERROR: thread1():qip_new_server()\n");
		return NULL;
	}

	do {
		size_t	len = 0;

		while (!qip_data_available(fd, 500))
			printf("Timeout!\n");

		if (buff)
			free(buff);

		if (peer)
			qip_free_peer(peer);

		if ((peer = qip_recv(fd, &buff, &len)) == NULL) {
			fprintf(stderr, "ERROR: thread1():qip_recv()\n");
			return NULL;
		}
	} while (strcmp(buff, "die"));

	printf("Ok, I will die but you will die too!\n");

	if (qip_send(fd, peer, "die", strlen("die")) < 0) {
		fprintf(stderr, "ERROR: thread1():qip_send()\n");
		return NULL;
	}

	qip_free_peer(peer);
	qip_close(fd);	
	return NULL;
}

void *
thread2(void *notused)
{
	qip_peer_t	*peer = NULL;
	char		*buff = NULL;
	int		fd = 0;

	if ((fd = qip_new_client()) < 0) {
		fprintf(stderr, "ERROR: thread1():qip_new_server()\n");
		return NULL;
	}

	if ((peer = qip_new_peer(LOCAL_NODE, THR1_PORT)) == NULL) {
		fprintf(stderr, "ERROR: thread2():qip_new_peer()\n");
		return NULL;
	}

	if (qip_send(fd, peer, "die", strlen("die")) < 0) {
		fprintf(stderr, "ERROR: thread1():qip_send()\n");
		return NULL;
	}
	qip_free_peer(peer);

	do {
		size_t	len = 0;

		if (buff)
			free(buff);

		if ((peer = qip_recv(fd, &buff, &len)) == NULL) {
			fprintf(stderr, "ERROR: thread1():qip_recv()\n");
			return NULL;
		}
		qip_free_peer(peer);
	} while (strcmp(buff, "die"));

	printf("Dying!!\n");
	qip_close(fd);	
	return NULL;
}
int
main(int argc, char *argv[])
{
	pthread_t	p1, p2;

	pthread_create(&p1, NULL, thread1, NULL);
	sleep(5);
	pthread_create(&p2, NULL, thread2, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	printf("Test Ok\n");

	return 0;
}
