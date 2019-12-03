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
#include <assert.h>

#include "../inc/main.h"
#include "../inc/http.h"
#include "../inc/test.h"

void
test_http_parse_request_line(unsigned u) {
	test_print(__func__, "id:%u", u);
	return;
}
void
test_http_parse_header(unsigned u) {
	test_print(__func__, "id:%u", u);
	const char *headers[] = {
		"Header: content",
		"Key: value",
		"Multiple: value1, value2, value3;",
		"SingleString",
		": EmptyVal",
		"EmptyKey: ",
	};
	const char *results[][2] = {
		{ "Header", "content", },
		{ "Key",    "value",   },
		{ "Multiple", "value1, value2, value3;", },
		{ "SingleString", "", },
		{ "",     "EmptyVal", },
		{ "EmptyKey",     "", },
	};
	for(int i=0; i<LENGTH(headers); i++) {
		const http_header_t *header = http_parse_header(headers[i]);
		assert(!strncmp(header->name,  results[i][0], sizeof(header->name)));
		assert(!strncmp(header->value, results[i][1], sizeof(header->value)));
		continue;
	}
	return;
}
void
test_http_parse_headers(unsigned u) {
	test_print(__func__, "id:%u", u);
	const char *headers[][5] = {
		{
			"Host: localhost:8000",
			"User-Agent: curl/7",
			"Connection: close",
		},
		{
			"Host: virtualhost",
			"User-Agent: Mozilla/5.0",
			"Connection: keep-alive",
		},
	};
	for(int i=0; i<LENGTH(headers); i++) {

		continue;
	}
	return;
}

int main(int argc, char **argv) {
	test_print(__func__, "%s:%d", argv[0], argc);
	test_proto_f tests[] = {
		&test_http_parse_request_line,
		&test_http_parse_header,
		&test_http_parse_headers,
		NULL,
	};
	for(int i=0; i<LENGTH(tests) && (tests[i]); i++) {
		if(tests[i]) test_run(i, tests[i]);
		continue;
	}
	list_t list;
	return 0;
}
