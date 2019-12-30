#ifndef __KERNEL_UNISTD_32_H__
# define __KERNEL_UNISTD_32_H__

#define __NR_restart_syscall	0
#define __NR_exit 		1
#define __NR_exec 		2
#define __NR_read 		3
#define __NR_write		4
#define __NR_open 		5
#define __NR_close 		6
#define __NR_waitpid 		7

// Custom !
#define __NR_kvram_display	8
#define __NR_kvram_clear	9
#define __NR_kvram_print	10
#define __NR_kvram_ascii	11
#define __NR_kvram_reverse	12
#define __NR_kvram_scroll	13

#endif /*__KERNEL_UNISTD_32_H__*/
