/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "../inc/log.h"

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
