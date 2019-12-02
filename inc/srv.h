
#ifndef _SRV_H_
#define _SRV_H_

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SRV_BUFFER_SIZE (1*1024*1024) // 1048576 = 1M

typedef struct sockaddr    sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;

typedef struct srv_conenction {
	sockaddr_in_t inaddr;
	sockaddr_t addr;
	size_t length;
	int fd;
} srv_connection_t;

void srv(void);
void srv_init(const int, const unsigned short);

#endif
