/*bin/echo
FILE="$(basename "$0").exe"
trap "rm -f '$FILE'" EXIT
cd "$(dirname "$0")"
cc -I../inc -o "$FILE" \
	"$(basename "$0")" \
	"../src/$(basename "$0")" \
	&& env "$FILE"
exit
*/

#if defined(NDEBUG)
#undef NDEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "../inc/main.h"
#include "../inc/file.h"
#include "../inc/test.h"

void
file_helper_create(const char *name, const char *contents) {
	FILE *fp = fopen(name, "wb");
	if(fp) {
		if(contents) {
			fwrite(contents, strlen(contents), sizeof(*contents), fp);
		}
		fclose(fp);
	}
	return;
}

void
test_file_size(unsigned u) {
	const char name[] = "testfile";
	file_helper_create(name, NULL);
	assert(0 == file_size(name));
	file_helper_create(name, "helloworld");
	assert(10 == file_size(name));
	file_helper_create(name,
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaa"
	);
	assert(25*10 == file_size(name));
	unlink(name);
	return;
}

int main(int argc, char **argv) {
	test_print(__func__, "%s:%d", argv[0], argc);
	test_proto_f tests[] = {
		&test_file_size,
		NULL,
	};
	for(int i=0; i<LENGTH(tests) && (tests[i]); i++) {
		if(tests[i]) test_run(i, tests[i]);
		continue;
	}
	return 0;
}
