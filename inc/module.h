/*bin/echo
cc -E $0
exit
*/

#ifndef _MODULE_H_
#define _MODULE_H_

// module: loaded at initialization

typedef struct module {
	char name[80];
	unsigned priority;
} module_t;

#endif
