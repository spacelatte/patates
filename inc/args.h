/*bin/echo
cc -E $0
exit
*/

#ifndef _ARGS_H_
#define _ARGS_H_

#include "map.h"

#define ARGS_SEPERATORS ":="

void args_init(void);

const char**  args_split(const char*);
const map_t** args_parse(const int, const char**);

void args_free(const map_t**);

const char* args_get(const map_t**, const char*);
const char* args_get_default(const map_t**, const char*, const char*);

#endif
