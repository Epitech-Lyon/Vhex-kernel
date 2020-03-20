#include <kernel/util/unistd_32.h>
.text
.global	_fork
.type	_fork, @function


.align 2
_fork:
	!trapa	#__NR_fork
	mov	#-1, r0
	rts
	nop
.end
