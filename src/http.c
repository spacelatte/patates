/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include "main.h"
#include "list.h"
#include "http.h"

static const char http_header_delimiters[] = " :;,";

static const char *http_line_delimiters[] = {
	"\r\n\r\n",
	"\r\r",
	"\n\n",
	NULL,
};

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
http_init(void) {
	return;
}

const http_request_line_t
http_parse_request_line(const char *line) {
	return (const http_request_line_t) { };
}

const http_header_t*
http_parse_header(const char *line) {
	if(!line) return NULL;
	http_header_t *result = ALLOC(1, http_header_t);
	//(http_header_t*) calloc(1, sizeof(http_header_t)); // TODO: use ALLOC
	size_t position = strcspn(line, http_header_delimiters);
	size_t offset = strspn(line+position, http_header_delimiters);
	strncpy(result->value, line+position+offset, sizeof(result->value));
	strncpy(result->name, line, position);
	//raise(SIGINT);
	return (const http_header_t*) result;
}

const list_t*
http_parse_headers(const char **headers) {
	const list_t *list = list_create(NULL);
	for(char **tmp=(char**) headers; tmp && *tmp; tmp++) {
		const http_header_t *hdr = http_parse_header(*tmp);
		list_append(&list, list_create((void*) hdr));
		continue;
	}
	return list;
}

http_t*
http_parse_request(const char **lines) {
	http_t http;
	return NULL;
}

//const char**
typeof(http_parse_raw(NULL))
http_parse_raw(const char *buffer) {
	char *body = NULL;
	for(
		char **sel=(char**) http_line_delimiters;
		!(body = strstr(buffer, *sel)) && sel && *sel;
		sel++
	);
	int nchars = strspn(body, *http_line_delimiters);
	body[nchars-1] = '\0';
	body += nchars;

	printf("body: .%s.\n", body);
	printf("head: .%s.\n", buffer);
	//return (const char**) { &buffer, &body, };
	return ( (typeof(http_parse_raw(NULL))) { .in = buffer, .body = body });
}


const char*
http_handle_request(const char *request) {
	return NULL;
}

/*



*/
