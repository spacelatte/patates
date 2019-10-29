
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "log.h"

void
logger(void);

void
log_print(FILE *fp, unsigned code, char level, const char *format, ...) {
	return;
}

void
log_error(unsigned code, const char *format, ...) {
	return;
}

void
log_init(const char *filename) {
	return;
}
