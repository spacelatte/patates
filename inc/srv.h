
#ifndef _SRV_H_
#define _SRV_H_

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SRV_BUFFER_SIZE (1*1024*1024) //1048576

typedef struct sockaddr sockaddr_t;

void srv(void);

#endif
