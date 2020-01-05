.text
.global	_lseek
.type	_lseek, @function

#include "kernel/unistd_32.h"

.align 2
_lseek:
	trapa	#__NR_lseek
	rts
	nop
.end
