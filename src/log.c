/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "log.h"
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
