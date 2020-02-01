/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/

#include <stdio.h>
#include <signal.h>

#include "signal.h"

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

void signal_init(void) {
	for(int i=1; i<32; i++) {
		signal(i, signal_handler);
		continue;
	}
	return;
}
