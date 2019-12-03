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
#include <stdlib.h>
#include <assert.h>

#include "../inc/main.h"
#include "../inc/args.h"
#include "../inc/test.h"

void
test_args_split(unsigned u) {
	test_print(__func__, "id:%u", u);
	const char *inputs[] = {
		"keyE=value",
		"keyC:value",
		"plainstr",
		"keyonlyE=",
		"keyonlyC:",
		"=valueE",
		":valueC",
		"=",
		":",
	};
	const char *outputs[][2] = {
		{ "keyE",     "value", },
		{ "keyC",     "value", },
		{ "plainstr", NULL,    },
		{ "keyonlyE", NULL,    },
		{ "keyonlyC", NULL,    },
		{ "valueE",   NULL,    },
		{ "valueC",   NULL,    },
		{ NULL,       NULL,    },
		{ NULL,       NULL,    },
	};
	for(int i=0; i<(sizeof(inputs)/sizeof(*inputs)); i++) {
		const char **result = args_split(inputs[i]);
		test_print(__func__, "<%-8s> | <%-8s> | <%-8s> | <%-8s>",
			result[0], outputs[i][0],
			result[1], outputs[i][1],
			NULL
		);
		assert(outputs[i][0] == result[0] || !strcmp(outputs[i][0], result[0]));
		assert(outputs[i][1] == result[1] || !strcmp(outputs[i][1], result[1]));
		free(result);
		continue;
	}
	return;
}

void
test_args_parse(unsigned u) {
	test_print(__func__, "id:%u", u);
	const char *argv[] = {
		"/path/to/binary",
		"paramC:valueC",
		"paramE=valueE",
		"dummy",
		"",
	};
	const map_t **args = args_parse(sizeof(argv)/sizeof(*argv), argv);
	assert(!strcmp("valueC", args_get(args, "paramC")));
	assert(!strcmp("valueE", args_get(args, "paramE")));
	assert(!strlen(args_get(args, "dummy")));
	return;
}

void
test_args_free(unsigned u) {
	return;
}

void
test_args_get(unsigned u) {
	return;
}

void
test_args_get_default(unsigned u) {
	return;
}

int main(int argc, char **argv) {
	test_print(__func__, "%s:%d", argv[0], argc);
	test_proto_f tests[] = {
		&test_args_split,
		&test_args_parse,
		&test_args_free,
		&test_args_get,
		&test_args_get_default,
		NULL,
	};
	for(int i=0; i<LENGTH(tests) && (tests[i]); i++) {
		if(tests[i]) test_run(i, tests[i]);
		continue;
	}
	return 0;
}
