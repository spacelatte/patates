
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#include "srv.h"

void 
srv(void) {
	return;
}

void
srv_handler(const srv_connection_t *conn) {
	if(!conn) return;
	char *buffer = (char*) calloc(SRV_BUFFER_SIZE, sizeof(char));
	if(!recv(conn->fd, buffer, SRV_BUFFER_SIZE, 0)) return;
	//if(!read(conn->fd, buffer, SRV_BUFFER_SIZE)) return;
	fprintf(stderr, "%s\n", buffer);
	snprintf(buffer, SRV_BUFFER_SIZE,
		"HTTP/1.1 200 OK\n"
		"Content-Type: text/plain\n"
		"Connection: close\n"
		"\n"
		"Hello World\n",
		NULL
	);
	send(conn->fd, buffer, strlen(buffer), 0);
	close(conn->fd);
	return;
}

const srv_connection_t*
srv_connection_copy(const srv_connection_t conn) {
	srv_connection_t *ptr = (srv_connection_t*) calloc(1, sizeof(conn));
	memcpy(ptr, &conn, sizeof(srv_connection_t));
	return (const srv_connection_t*) ptr;
}

const pthread_t
srv_loop(const int socket) {
	sockaddr_t addr;
	socklen_t length;
	int fd = accept(socket, &addr, &length);
	const srv_connection_t *conn = srv_connection_copy((srv_connection_t) {
		.inaddr = {},
		.addr   = addr,
		.length = length,
		.fd     = fd,
	});
	pthread_t thread;
	pthread_create(&thread, NULL, &srv_handler, (void*) conn);
	return thread;
}

void
srv_init(const int count, const unsigned short port) {
	const sockaddr_in_t server = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = htonl(INADDR_ANY),
		},
	};
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
	int bindfd = bind(sockfd, (const sockaddr_t*) &server, sizeof(server));
	listen(sockfd, count);
	while(bindfd >= 0) {
		(srv_loop(sockfd), NULL);
		continue;
	}
	close(sockfd);
	return;
}
