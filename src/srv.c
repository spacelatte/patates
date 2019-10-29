
#include <pthread.h>

#include "srv.h"

void 
srv(void) {
	return;
}

void
srv_handler(const int sockfd) {
	char *buffer = (char*) calloc(SRV_BUFFER_SIZE, sizeof(char));

	return;
}

void
srv_loop(int socket) {
	sockaddr_t addr;
	socklen_t length;
	int fd = accept(socket, &addr, &length);
	return;
}

void
srv_init(void) {
	return;}
