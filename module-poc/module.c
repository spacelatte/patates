
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <dlfcn.h>


void print(const char *fmt, ...) {
	unsigned long length = strlen(fmt);
	char *buffer = (char*) calloc(length, sizeof(char));
	strncpy(buffer, fmt, length);
	strncat(buffer, "\n", sizeof(char));
	va_list args;
	va_start(args, fmt);
	vprintf(buffer, args);
	va_end(args);
	free(buffer);
	return;
}

__attribute__((constructor)) void construct(void) {
	print("construct");
	usleep(1000L);
	return;
}

__attribute__((destructor)) void destruct(void) {
	print("destruct");
	usleep(1000L);
	return;
}

void tester(void) {
	printf("THREAD: %p\n", pthread_self());
	return;
}

void msleep(unsigned long msecs) {
	usleep(msecs * 1000L);
	return;
}

void *thread_task(void *param) {
	time_t now = time(NULL);
	char *timestr = ctime(&now);
	timestr[strlen(timestr)-2] = '\0';
	print("Time: %s", timestr);
	//msleep(666);
	void (*function)(void);
	function = param;
	if(function) function();
	return NULL; // calloc(102400L, 102400L);
}

int main(int argc, char **argv) {
	print("Hello %s %d", "asdasdaaaaaaaaaaaaaaaaaaaaa", argc);
	pthread_t thread;
	pthread_create(&thread, NULL, &thread_task, &tester);
	msleep(666);
	return 0;
}
