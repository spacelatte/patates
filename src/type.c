/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>

#include "type.h"

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
type_init(void) {
	return;
}
