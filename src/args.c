
#include <stdlib.h>
#include <string.h>

#include "args.h"

void
args(void) {
	return;
}

const char**
args_split(const char *arg) {
	const char **pair = calloc(2, sizeof(char*));
	pair[0] = strtok((char*) arg,  ":=");
	pair[1] = strtok((char*) NULL, ":=");
	return pair;
}

const map_t**
args_parse(const int argc, const char **argv) {
	map_t **args = (map_t**) calloc(argc + 1, sizeof(map_t*));
	for(int i=0; i<argc; i++) {
		// TODO: copy is inefficient, convert to passing pointers
		args[i] = (map_t*) calloc(1, sizeof(map_t));
		const char **pair = args_split(argv[i]);
		if(pair[0]) strncpy(args[i]->key, pair[0], sizeof(args[i]->key));
		if(pair[1]) strncpy(args[i]->val, pair[1], sizeof(args[i]->val));
		free(pair);
		continue;
	}
	args[argc] = NULL;
	return (const map_t**) args;
}

void
args_free(const map_t **args) {
	for(map_t **tmp=(map_t**) args; tmp && *tmp; tmp++) {
		free(*tmp);
	}
	free(args);
	return;
}

const char*
args_get(const map_t **args, const char *key) {
	for(map_t **tmp=(map_t**) args; tmp && *tmp; tmp++) {
		// TODO: buggy?
		if(!strncmp((*tmp)->key, key, sizeof((*tmp)->key))) {
			return (*tmp)->val;
		}
		continue;
	}
	return NULL;
}
