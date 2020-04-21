#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

// TODO: libc
const char *signal_str[NSIG] = {
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	NULL,
	"SIGFPE",
	"SIGKILL",
	"SIGBUS",
	"SIGSEGV",
	"SIGSYS",
	"SIGPIPE",
	"SIGALARM",
	"SIGURG",
	"SIGSTOP",
	"SIGTSTP",
	"SIGCONT",
	"SIGCHLD",
	"SIGTTIN",
	"SIGTTOU",
	"SIGPOLL",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGUSR1",
	"SIGUSR2"
};

void wait_child(pid_t pid)
{
	int wstatus;

	// Wait child death
	waitpid(pid, &wstatus, 0);

	// FIXME: workaround dto display
	printf("\n");

	// Check exit() return
	if (WIFEXITED(wstatus))
	{
		wstatus = WEXITSTATUS(wstatus);
		if (wstatus != 0)
			printf("[%d]", wstatus);
		return;
	}

	// Check sigals
	if (WIFSTOPPED(wstatus))
	{
		wstatus = WSTOPSIG(wstatus) - 1;
		if (wstatus < 0 || wstatus >= NSIG) {
			printf("+++ unknown signal %d +++\n", wstatus);
			return;
		}
		printf("[%s]", signal_str[wstatus]);
		return;
	}

	// Error, unknown
	printf("+++ unknown value %d +++\n", wstatus);
}
