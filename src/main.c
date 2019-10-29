
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

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
main(int argc, char **argv) {
	map_t **args = args_parse(argc, argv);
	for(map_t **tmp=args; tmp && *tmp; tmp++) {
		printf("%s: %s\n", (*tmp)->key, args_get(args, (*tmp)->key));
		continue;
	}
	srv_init(10, 8080);
	cleanup: {
		args_free(args);
	}
	return 0;
}
