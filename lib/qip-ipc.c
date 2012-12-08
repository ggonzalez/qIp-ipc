#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "qip-ipc.h"

int
qip_new_client()
{
	struct sockaddr_in	s;
	int	ret = 0;

	if ((ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("qip_new_client():socket()");
		ret = -1;
	}

	return ret;
}

int
qip_new_server(int port)
{
	struct sockaddr_in	s;
	int			ret = 0;

	if ((ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("qip_new_client():socket()");
		return -1;
	}

	memset(&s, 0, sizeof(s));
	s.sin_family = AF_INET;
	s.sin_port = htons(port);
	s.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(ret, (struct sockaddr *)&s, sizeof(s)) < 0) {
		perror("qip_new_server():bind()");
		close(ret);
		ret = -1;
	}

	return ret;	
}

int
qip_close(int fd)
{
	return close(fd);
}

int
qip_data_available(int fd, time_t tout_ms)
{
	struct timeval	tout;
	fd_set		rfds;
	int		ret = 0;

	tout.tv_sec = 0;
	tout.tv_usec = tout_ms * 1000;

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	switch(select(fd + 1, &rfds, NULL, NULL, &tout)) {
	case -1:
		perror("qip_data_available():select");	
		ret = 0;
		break;
	case 0:
		ret = 0;
		break;
	default:
		if (FD_ISSET(fd, &rfds)) {
			ret = 1;
		} else {
			ret = 0;
		}
	}

	return ret;
}

qip_peer_t *
qip_new_peer(const char *addr, int port)
{
	qip_peer_t	*peer = NULL;

	if ((peer = (qip_peer_t *)calloc(1, sizeof(qip_peer_t))) == NULL) {
		perror("qip_new_peer():calloc()");
		return NULL;
	}

	((struct sockaddr_in *)&(peer->s))->sin_family = AF_INET;
	((struct sockaddr_in *)&(peer->s))->sin_port = htons(port);
	((struct sockaddr_in *)&(peer->s))->sin_addr.s_addr = inet_addr(addr);

	return peer;
}

void
qip_free_peer(qip_peer_t *peer)
{
	if (peer)
		free(peer);
}

qip_peer_t *
qip_recv(int fd, char **buff, size_t *len)
{
	qip_peer_t	*ret = NULL;
	size_t		tmplen = 0;

	if ((ret = (qip_peer_t *)calloc(1, sizeof(qip_peer_t))) == NULL) {
		perror("qip_recv():calloc(1)");
		return NULL;
	}

	/*
	 * Len First
	 */
	if (recvfrom(fd, (char *)len, sizeof(len), 0, &ret->s, &tmplen) < 0) {
		perror("qip_recv():recvfrom(1)");
		free(ret);
		return NULL;
	}

	if ((*buff = (char *)calloc(*len, sizeof(char))) == NULL) {
		perror("qip_recv():calloc(2)");
		free(ret);
		return NULL;
	}

	if (recvfrom(fd, (char *)*buff, *len, 0, &ret->s, &tmplen) < 0) {
		perror("qip_recv():recvfrom(2)");
		free(ret);
		return NULL;
	}

	return ret;
}

int
qip_send(int fd, qip_peer_t *peer, const char *buf, size_t len)
{
	int	ret = 0;

	/*
	 * First send length
	 */
	if (sendto(fd, (char *)&len, sizeof(len), 0, &peer->s, sizeof(peer->s)) < 0) {
		perror("qip_send():sendto()");
		return -1;
	}

	/*
	 * Second send buff
	 */
	if (sendto(fd, buf, len, 0, &peer->s, sizeof(peer->s)) < 0) {
		perror("qip_send():sendto()");
		return -1;
	}

	return ret;
}
