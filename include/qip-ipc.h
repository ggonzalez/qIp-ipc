#ifndef _QIPIPC_H_
#define _QIPIPC_H_

#include <netinet/in.h>

typedef struct {
	struct sockaddr	s;
} qip_peer_t;

int		qip_new_client();
int		qip_new_server(int port);

int		qip_data_available(int fd, time_t tout_ms);

qip_peer_t	*qip_new_peer(const char *addr, int port);
void		qip_free_peer(qip_peer_t *);

qip_peer_t	*qip_recv(int fd, char **buf, size_t *len);
int		qip_send(int fd, qip_peer_t *peer, const char *buf, size_t len);

int		qip_close(int fd);

#endif /* _QIP-IPC_H_ */
