
#ifndef _ARGS_H_
#define _ARGS_H_

typedef struct Map {
	char key[80];
	char val[1024];
} map_t;

void args(void);

const map_t** args_parse(const int, const char**);
void args_free(const map_t**);

const char* args_get(const map_t**, const char*);

#endif
