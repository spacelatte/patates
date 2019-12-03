/*bin/echo
cc -E $0
exit
*/

#ifndef _LIST_H_
#define _LIST_H_

#include <stdbool.h>

typedef struct List {
	struct List *prev;
	struct List *next;
	char format; // TODO: printf style format specifier
	void *data;
} list_t;

typedef bool (*list_each_f)(list_t*);
typedef void (*list_each_data_f)(void*);

void list(void);

const list_t* list_create(void*);

const list_t* list_head(const list_t**);
const list_t* list_tail(const list_t**);

void list_prepend(const list_t**, const list_t*);
void list_append(const list_t**, const list_t*);

void list_each(const list_t**, list_each_f);
void list_each_data(const list_t**, list_each_data_f);

void list_free(const list_t**);
void list_free_data(const list_t**);

const list_t* list_circular(const list_t**);
const list_t* list_break(const list_t**);

const list_t* list_get(const list_t**, unsigned long);
unsigned long list_size(const list_t**);

const list_t* list_find_next(const list_t**, const list_t*);
const list_t* list_unlink(const list_t**, const list_t*);

#endif
