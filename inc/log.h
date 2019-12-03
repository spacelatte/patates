/*bin/echo
cc -E $0
exit
*/

#ifndef _LOG_H_
#define _LOG_H_

#include <stddef.h>
#include <stdarg.h>

typedef void (*log_print_f)(unsigned, char, const char*, ...);

typedef enum log_level {
	LOG_LEVEL_NONE    = 0,
	LOG_LEVEL_ERROR   = 1,
	LOG_LEVEL_WARN    = 2,
	LOG_LEVEL_INFO    = 3,
	LOG_LEVEL_VERBOSE = 4,
	LOG_LEVEL_DEBUG   = 5,
	LOG_LEVEL_COUNT   = 6,
} log_level_e;

const char *log_level_chr = "?EWIVD";

void logger(void);

#endif
