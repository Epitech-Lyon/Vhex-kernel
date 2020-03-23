#include <kernel/process.h>

//TODO: free allocated proc ?
int process_free(struct process *process)
{
	extern struct process *proc_table;
	extern uint32_t proc_table_max;
	uint32_t proc_table_idx;
	struct process *proc;

	proc = proc_table;
	proc_table_idx = 0;
	while (proc != NULL && ++proc_table_idx < proc_table_max)
	{
		if (proc == process)
		{
			proc->status = PROC_DEAD;
			return (0);
		}
	}
	return (-1);
}
