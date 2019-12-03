/*bin/echo
cc -E $0
exit
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#define ALLOC(COUNT, TYPE)        ((TYPE*) calloc((COUNT), sizeof(TYPE)))
#define RALLOC(ORIG, COUNT, TYPE) ((TYPE*) realloc((ORIG), (COUNT) * sizeof(TYPE)))
#define LENGTH(ARRAY) (sizeof(ARRAY)/sizeof(*(ARRAY)))

typedef char *str;

#endif

#ifndef MAIN_NAME
#define MAIN_NAME "patates"
#endif

#ifndef MAIN_VER
#define MAIN_VER  "1.0"
#endif
