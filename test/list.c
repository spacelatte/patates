
#if defined(NDEBUG)
#undef NDEBUG
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "list.h"

const unsigned FN_PAD_SIZE = 24;

typedef void (*test_proto_f)(unsigned);

void print(const char *fn, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char buffer[1024 + strlen(fmt)];
	snprintf(buffer, sizeof(buffer), "Testing: %*s: %s\n", FN_PAD_SIZE, fn, fmt);
	vfprintf(stdout, buffer, args);
	va_end(args);
	return;
}

void
test_create(unsigned u) {
	print(__func__, "id:%u", u);
	const list_t *list = list_create((void*) __func__);
	assert(!(list == NULL));
	assert(!strcmp(list->data, __func__));
	return;
}

void
test_list_append(unsigned u) {
	print(__func__, "id:%u", u);
	const list_t *list = list_create((void*) __func__);
	const list_t *next = list_create((void*) __func__);
	const list_t *tail = list_create((void*) __func__);
	// initial
	list_append(&list, next);
	assert(next == list->next);
	// add another
	list_append(&next, tail);
	assert(tail == next->next);
	// make circular
	list_append(&tail, list);
	assert(list == list->next->next->next);
	assert(next == list->next->next->next->next);
	assert(tail == list->next->next->next->next->next);
	assert(list == list->next->next->next->next->next->next);
	assert(next == list->next->next->next->next->next->next->next);
	assert(tail == list->next->next);
	return;
}

void
test_list_prepend(unsigned u) {
	print(__func__, "id:%u", u);
	const list_t *list = list_create((void*) __func__);
	const list_t *next = list_create((void*) __func__);
	const list_t *tail = list_create((void*) __func__);
	// initial
	list_prepend(&list, next);
	assert(list == next);
	list_prepend(&list, tail);
	assert(list == tail);
	return;
	// add another
	list_prepend(&next, tail);
	assert(tail == next->next);
	// make circular
	list_prepend(&tail, list);
	assert(list == list->next->next->next);
	assert(next == list->next->next->next->next);
	assert(tail == list->next->next->next->next->next);
	assert(list == list->next->next->next->next->next->next);
	assert(next == list->next->next->next->next->next->next->next);
	assert(tail == list->next->next);
	return;
}

int main(int argc, char **argv) {
	print(__func__, "%s:%d", argv[0], argc);
	test_proto_f tests[] = {
		&test_create,
		&test_list_append,
		&test_list_prepend,
		NULL,
	};
	for(int i=0; i<(sizeof(tests)/sizeof(tests[0])) && (tests[i]); i++) {
		if(tests[i]) (tests[i])(i);
		continue;
	}
	return 0;
}
