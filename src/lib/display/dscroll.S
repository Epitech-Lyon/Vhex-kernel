#include <kernel/util/unistd_32.h>
.text
.global	_dscroll
.type	_dscroll, @function


.align 2
_dclear:
	trapa	#__NR_kvram_scroll
	rts
	nop
.end
