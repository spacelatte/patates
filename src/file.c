/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>

#include "../inc/file.h"

size_t
file_size(const char *name) {
	FILE *fp = fopen(name, "rb");
	fseek(fp, 0L, SEEK_END);
	size_t size = ftell(fp);
	fclose(fp);
	return size;
}
