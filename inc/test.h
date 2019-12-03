/*bin/echo
cc -E $0
exit
*/

#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

const unsigned TEST_PAD = -24;

typedef void (*test_proto_f)(unsigned);

void test_print(const char *fn, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char buffer[1024 + strlen(fmt)];
	snprintf(buffer, sizeof(buffer), "Testing: %*s: %s\n", -TEST_PAD, fn, fmt);
	vfprintf(stdout, buffer, args);
	va_end(args);
	return;
}

void test_run(unsigned u, test_proto_f fn) {
	return fn(u);
}

#endif
