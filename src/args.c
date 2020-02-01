/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "args.h"

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
args_init(void) {
	return;
}

const char**
args_split(const char *arg) {
	if(!arg) return NULL;
	unsigned count = 0;
	const char *tmp, *state, *copy = strdup(arg);
	const char **pair = ALLOC(1, const char*);
	//(const char*) calloc(1, sizeof(char*)); // TODO: use ALLOC
	pair[count++] = strtok_r((char*) copy, ARGS_SEPERATORS, (char**) &state);
	while(true
		&& (tmp = strtok_r((char*) NULL, ARGS_SEPERATORS, (char**) &state))
		//&& (pair = realloc(pair, (count * sizeof(char*))))
		&& (pair = RALLOC(pair, count, const char*))
	) {
		pair[count++] = tmp;
		continue;
	}
	free((char*) copy);
	return pair;
}

const map_t**
args_parse(const int argc, const char **argv) {
	map_t **args = ALLOC(argc+1, map_t*);
	//(map_t**) calloc(argc + 1, sizeof(map_t*)); // TODO: use ALLOC
	for(int i=0; i<argc; i++) {
		// TODO: copy is inefficient, convert to passing pointers
		args[i] = ALLOC(1, map_t);
		//(map_t*) calloc(1, sizeof(map_t)); // TODO use ALLOC
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

const char*
args_get_default(const map_t **args, const char *key, const char *def) {
	const char *value = args_get(args, key);
	return value ? value : def;
}
