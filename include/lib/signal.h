#ifndef __LIB_SIGNAL_H__
# define __LIB_SIGNAL_H__

#include <stddef.h>
#include <stdint.h>
#include <bits/signum.h>

/* Type of a signal handler.  (kernel / user) */
typedef void (*__sighandler_t)(int);
typedef __sighandler_t sighandler_t;

// Prototype
extern int kill(pid_t pid, int sig);
extern sighandler_t signal(int signum, sighandler_t handler);

#endif /*__LIB_SIGNAL_H__*/
