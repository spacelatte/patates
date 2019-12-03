/*bin/echo
cc -E $0
exit
*/

#ifndef _HTTP_H_
#define _HTTP_H_

#include "./list.h"

typedef union http_version {
	char str[16]; // raw
	struct {
		char http[4];
		char slash[1];
		char version[4];
		struct {
			unsigned short major;
			unsigned short minor;
		}; // semver ?
	};
} http_version_t;

typedef struct http_request_line {
	char method[16];
	char uri[16384];
	http_version_t version;
} http_request_line_t;

typedef struct http_status_line {
	http_version_t version;
	union {
		short num; // as number?
		char str[4]; // with null
	} code;
	char message[64];
} http_status_line_t;

typedef struct http_header {
	char  name[1024];
	char value[8192];
	unsigned long numeric;
} http_header_t;

typedef struct HTTP {
	union {
		http_request_line_t request_line;
		http_status_line_t  status_line;
	};
	list_t *headers;
	unsigned char *body;
} http_t;


//extern const char *http_delimiters[];

void http(void);

const char** http_parse_raw(const char*);

const http_header_t* http_parse_header(const char*);


#endif
