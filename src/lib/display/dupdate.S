.text
.global	_dupdate
.type	_dupdate, @function

#include "kernel/unistd_32.h"

.align 2
_dupdate:
	trapa	#__NR_kvram_display
	rts
	nop
.end
