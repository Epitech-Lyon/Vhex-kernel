#ifndef __KERNEL_SIGNAL_H__
# define __KERNEL_SIGNAL_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/process.h>

// Internal
extern int signal_raise(struct process *proc, int sig);
extern int signal_deliver_pending(struct process *proc);

#endif /*__KERNEL_SIGNAL_H__*/
