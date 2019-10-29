
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

void
list(void) {
	return;
}

const list_t*
list_create(void *data) {
	list_t *list = (list_t*) calloc(1, sizeof(list_t));
	list->data = data;
	return list;
}

const list_t*
list_head(const list_t **list) {
	if(!list || !*list) return NULL;
	list_t *tmp = (list_t*)(*list);
	while(tmp && tmp->prev && *list != tmp->prev) {
		tmp = tmp->prev;
		continue;
	}
	return tmp;
}

const list_t*
list_tail(const list_t **list) {
	if(!list || !*list) return NULL;
	list_t *tmp = (list_t*)(*list);
	while(tmp && tmp->next && *list != tmp->next) {
		tmp = tmp->next;
		continue;
	}
	return tmp;
}

void
list_prepend(const list_t **dst, const list_t *src) {
	if(!dst || !*dst || !src) return;
	((list_t*) list_tail(&src))->next = (list_t*) *dst;
	*dst = src;
	return;
}

void
list_append(const list_t **dst, const list_t *src) {
	if(!dst || !*dst || !src) return;
	list_t *tail = (list_t*) list_tail(dst);
	tail->next = (list_t*) src;
	return;
}

void
list_each(const list_t **list, list_each_f function) {
	if(!list || !*list || !function) return;
	for(list_t *tmp=(list_t*)(*list); tmp; tmp=tmp->next) {
		function(tmp);
		if(*list == tmp->next) {
			break;
		}
		continue;
	}
	return;
}

void
list_each_data(const list_t **list, list_each_data_f function) {
	if(!list || !*list || !function) return;
	for(list_t *tmp=(list_t*)(*list); tmp; tmp=tmp->next) {
		function(tmp->data);
		if(*list == tmp->next) {
			break;
		}
		continue;
	}
	return;
}

void
list_free(const list_t **list) {
	if(!list || !*list) return;
	const list_t *next = list_break(list);
	if(next) {
		list_free(&next);
	}
	free((void*) *list);
	*list = NULL;
	return;
}

void
list_free_data(const list_t **list) {
	if(!list || !*list) return;
	list_each_data(list, free);
	return;
}

const list_t*
list_circular(const list_t **list) {
	if(!list || !*list) return NULL;
	list_t *tmp = (list_t*)(*list);
	while(tmp && *list != tmp->next) {
		tmp = tmp->next;
		continue;
	}
	return tmp ? tmp->next : tmp;
}

const list_t*
list_break(const list_t **list) {
	if(!list || !*list) return NULL;
	list_t *tmp = (list_t*)(*list);
	list_t *next = tmp->next;
	tmp->next = NULL;
	return next;
}

const list_t*
list_get(const list_t **list, unsigned long n) {
	if(!list || !*list) return NULL;
	list_t *tmp = (list_t*)(*list);
	while(tmp && *list != tmp && n--) {
		tmp = tmp->next;
		continue;
	}
	return tmp;
}

unsigned long
list_size(const list_t **list) {
	if(!list || !*list) return 0;
	list_t *tmp = (list_t*)(*list);
	unsigned long count = 0;
	while(tmp && *list != tmp) {
		tmp = tmp->next;
		count += 1;
		continue;
	}
	return count;
}

const list_t*
list_find_next(const list_t **list, const list_t *next) {
	if(!list || !*list || !next) return NULL;
	list_t *tmp = (list_t*)(*list);
	while(tmp && tmp->next && *list != tmp->next && next != tmp->next) {
		tmp = tmp->next;
		continue;
	}
	return (next == tmp->next) ? tmp : NULL;
}

const list_t*
list_unlink(const list_t **list, const list_t *elem) {
	if(!list || !*list || !elem) return NULL;
	list_t *prev = (list_t*) list_find_next(list, elem);
	if(prev && prev->next && elem == prev->next) {
		prev->next = elem->next;
		return elem;
	}
	return NULL;
}
