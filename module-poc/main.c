
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <math.h>

typedef int (*mod_entry_f) (unsigned, ...);

const char *sigstr[] = {
	"?",
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGEMT",
	"SIGFPE",
	"SIGKILL",
	"SIGBUS",
	"SIGSEGV",
	"SIGSYS",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGURG",
	"SIGSTOP",
	"SIGTSTP",
	"SIGCONT",
	"SIGCHLD",
	"SIGTTIN",
	"SIGTTOU",
	"SIGIO",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGINFO",
	"SIGUSR1",
	"SIGUSR2",
};

const char *time_unit[] = {
	"seconds",
	"deciseconds",
	"centiseconds",
	"milliseconds|1",
	"milliseconds|10",
	"milliseconds|100",
	"microseconds|1",
	"microseconds|10",
	"microseconds|100",
	"nanoseconds|1",
	"nanoseconds|10",
	"nanoseconds|100",
};

void signal_handler(int signum) {
	fprintf(stderr, "Signal (%2d)%s Received!\n", signum, sigstr[signum]);
	switch(signum) {
		case 15:
		//case 11:
		case 3:
		case 2:
			exit(0);
			break;
	}
	switch(signum) {
		case 11:
			sleep(1);
			break;
	}
	return;
}

void *dynamic(char *name, int mode) {
	static void *original = NULL;
	void *current = dlopen(name, mode);
	if(current) {
		void *old = original;
		original = current;
		if(old) {
			dlclose(old);
		}
	}
	return original;
}

typedef struct ClockBenchmark {
	clock_t start;
	clock_t open;
	clock_t found;
	clock_t wall;
} clock_bench_t;

int main(int argc, char **argv) {
	if(argc < 2) {
		perror("module_name is required!");
		return 1;
	}
	for(int i=1; i<32; i++) {
		signal(i, signal_handler);
		continue;
	}
	const int time_unit_index = log10(labs(CLOCKS_PER_SEC));
	while(argc) {
		/*
		clock_t cfound, copen, cstart;
		cfound = copen = cstart = clock();
		clock_t cwall = clock();
		*/
		clock_bench_t bench = {
			.start = clock(),
			.open  = clock(),
			.found = clock(),
			.wall  = clock(),
		};
		void *mod = dlopen(argv[1], RTLD_LAZY | RTLD_LOCAL);
		if(mod) {
			bench.open = clock(); //copen = clock();
			mod_entry_f entryfn = dlsym(mod, "main");
			if(entryfn) {
				bench.found = clock(); //cfound = clock();
				entryfn(argc-1, argv+1);
			} else {
				fprintf(stderr, "Ercror at dlsym: %p, %s\n", entryfn, dlerror());
				sleep(1);
			}
			dlclose(mod);
		} else {
			fprintf(stderr, "Error at dlopen: %p %s\n", mod, dlerror());
			sleep(1);
			continue;
		}
		printf(
			"stats:"
			" wall:%ld"
			" open:%ld"
			" found:%ld"
			" delta:%ld"
			" | "
			"unit:%s"
			"(cps:%d)"
			"\n",
			//copen-cstart, cfound-cstart, cfound-copen, cwall-cstart,
			bench.wall  - bench.start,
			bench.open  - bench.start,
			bench.found - bench.start,
			bench.found - bench.open,
			time_unit[time_unit_index],
			CLOCKS_PER_SEC,
			NULL
		);
		continue;
	}
	return 0;
}
