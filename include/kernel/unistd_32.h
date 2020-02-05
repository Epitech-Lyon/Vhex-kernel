#ifndef __KERNEL_UNISTD_32_H__
# define __KERNEL_UNISTD_32_H__

// Kernel Test
#define __NR_test_syscall	0

// Process
#define __NR_exit 		1
#define __NR_fexecve 		2
#define __NR_waitpid 		3

// VFS
#define __NR_read 		4
#define __NR_write		5
#define __NR_open 		6
#define __NR_close 		7
#define __NR_lseek 		8

//--
//	Custom !
//--

// Display
#define __NR_kvram_display	9
#define __NR_kvram_clear	10
#define __NR_kvram_print	11
#define __NR_kvram_ascii	12
#define __NR_kvram_reverse	13
#define __NR_kvram_scroll	14
#define __NR_kvram_clr_str_area	15

//TODO: keyboard

#endif /*__KERNEL_UNISTD_32_H__*/
