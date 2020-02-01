/*bin/echo
FILE="$(basename "$0").exe"
trap "rm -f '$FILE'" EXIT
cd "$(dirname "$0")"
cc -I../inc -o "$FILE" \
	"$(basename "$0")" \
	"../src/$(basename "$0")" \
	&& env "$FILE"
exit
*/

#if defined(NDEBUG)
#undef NDEBUG
#endif

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "test.h"

void
test_list_create(unsigned u) {
	test_print(__func__, "id:%u", u);
	const list_t *list = list_create((void*) __func__);
	assert(!(list == NULL));
	assert(!strcmp(list->data, __func__));
	return;
}

void
test_list_append(unsigned u) {
	test_print(__func__, "id:%u", u);
	const list_t *list = list_create((void*) "1");
	const list_t *next = list_create((void*) "2");
	const list_t *tail = list_create((void*) "3");
	// initial
	list_append((const list_t**) &list, next);
	assert(next == list->next);
	// add another
	list_append((const list_t**) &next, tail);
	assert(tail == next->next);
	// make circular
	list_append((const list_t**) &tail, list);
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
	test_print(__func__, "id:%u", u);
	const list_t *list = list_create((void*) "1");
	const list_t *next = list_create((void*) "2");
	const list_t *tail = list_create((void*) "3");
	list_t *tmp = (list_t*) list;
	// initial
	list_prepend((const list_t**) &tmp, next);
	assert(next == tmp);
	assert(list == next->next);
	// add another
	list_prepend((const list_t**) &tmp, tail);
	assert(tail == tmp);
	assert(next == tmp->next);
	assert(list == tmp->next->next);
	// make circular
	list_prepend((const list_t**) &tmp, tmp);
	assert(tmp  == tmp->next->next->next);
	assert(tail == tmp->next->next->next);
	assert(list == tmp->next->next);
	assert(next == tmp->next);
	assert(tail == tmp);
	return;
}

int
main(int argc, char **argv) {
	test_print(__func__, "%s:%d", argv[0], argc);
	test_cycle((test_proto_f[]) {
		&test_list_create,
		&test_list_append,
		&test_list_prepend,
		NULL,
	});
	return 0;
}
