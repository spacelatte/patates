/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "main.h"

#include "args.h"
#include "list.h"

#include "srv.h"
#include "http.h"

bool
test(list_t *node) {
	printf("node:%p, data=%p, next=%p \n", node, node->data, node->next);
	return true;
}

int
main(const int argc, const char **argv) {
	const map_t **args = (const map_t**) args_parse(argc, argv);
	for(map_t **tmp=(map_t**) args; tmp && *tmp; tmp++) {
		printf("%s: %s\n", (*tmp)->key,
			args_get(args, (*tmp)->key)
		);
		continue;
	}
	srv_init(
		atoi(args_get_default(args, "queue", "10")),
		atoi(args_get_default(args, "port", "8080"))
	);
	goto cleanup;
	cleanup: {
		args_free(args);
	}
	return 0;
}
