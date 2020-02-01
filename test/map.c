
#if defined(NDEBUG)
#undef NDEBUG
#endif

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "test.h"


int
main(int argc, char **argv) {
	test_print(__func__, "%s:%d", argv[0], argc);
	test_cycle((test_proto_f[]) {
		NULL,
	});
	return 0;
}
