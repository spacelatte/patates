/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "main.h"
#include "http.h"
#include "srv.h"

__attribute__((constructor)) static void
construct(void) {
	fprintf(stderr, "%s: %s: %d\n", __FILE__, __FUNCTION__, __LINE__);
	return;
}

__attribute__((destructor)) static void destruct(void) {
	fprintf(stderr, "%s: %s: %d\n", __FILE__, __FUNCTION__, __LINE__);
	return;
}

void
srv_signal_handle(int s) {
	pthread_t thread = pthread_self();
	printf("signal %d on %p\n", s, thread);
	return pthread_exit(NULL);
}

void
srv_signal_init(sig_t handler, ...) {
	va_list args;
	va_start(args, handler);
	int i;
	while((i = va_arg(args, int))) {
		signal(i, handler);
		continue;
	}
	return;
}

void
srv_handler(const srv_connection_t *conn) {
	if(!conn) return;
	srv_signal_init(&srv_signal_handle,
		SIGHUP,
		SIGINT,
		SIGQUIT,
		SIGILL,
		SIGTRAP,
		SIGABRT,
		SIGFPE,
		SIGKILL,
		SIGBUS,
		SIGSEGV,
		SIGSYS,
		SIGPIPE,
		SIGALRM,
		SIGTERM,
		SIGURG,
		SIGSTOP,
		SIGTSTP,
		SIGCONT,
		SIGCHLD,
		SIGTTIN,
		SIGTTOU,
		SIGIO,
		SIGXCPU,
		SIGXFSZ,
		SIGVTALRM,
		SIGPROF,
		SIGWINCH,
		SIGUSR1,
		SIGUSR2,
		0, NULL
	);
	char *buffer = ALLOC(SRV_BUFFER_SIZE, char);
	//(char*) calloc(SRV_BUFFER_SIZE, sizeof(char)); // TODO: use ALLOC
	if(!recv(conn->fd, buffer, SRV_BUFFER_SIZE, 0)) return;
	http_parse_raw(buffer);
	free(buffer);
	close(conn->fd);
	return; ////////////////////////////////////////////////////////////////////
	unsigned long count = 0;
	char *tmp, **lines = NULL;
	while((tmp = strsep(&buffer, "\r\n"))) {
		printf("%lu %s\n", count, tmp);
		if(!tmp || !*tmp) continue;
		count += 1;
		lines = RALLOC(lines, count, char*);
		//(char**) realloc(lines, count * sizeof(char*)); // TODO: use RALLOC
		lines[count-1] = tmp;
		continue;
	}
	//raise(SIGINT);
	for(char **t = lines; t && *t; t++) printf("> %s\n", *t);
	//if(!read(conn->fd, buffer, SRV_BUFFER_SIZE)) return;
	snprintf(buffer, SRV_BUFFER_SIZE,
		"HTTP/1.1 200 OK\n"
		"Server: %s/%s\n"
		"Content-Type: text/plain\n"
		"Connection: close\n"
		"\n"  "Hello World\n",
		MAIN_NAME, MAIN_VER, NULL
	);
	send(conn->fd, buffer, strlen(buffer), 0);
	close(conn->fd);
	free(buffer);
	return pthread_exit(NULL);
}

const srv_connection_t*
srv_connection_copy(const srv_connection_t conn) {
	srv_connection_t *ptr = ALLOC(1, srv_connection_t);
	//(srv_connection_t*) calloc(1, sizeof(conn)); // TODO: use ALLOC
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
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) { 1 }, sizeof(int));
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &(int) { 1 }, sizeof(int));
	int bindfd = bind(sockfd, (const sockaddr_t*) &server, sizeof(server));
	listen(sockfd, count);
	while(bindfd-- >= 0) {
		pthread_join(srv_loop(sockfd), NULL);
		continue;
	}
	fflush(stdout);
	close(sockfd);
	return;
}
