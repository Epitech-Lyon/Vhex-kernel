#include <kernel/util/unistd_32.h>
.text
.global	_dupdate
.type	_dupdate, @function


.align 2
_dupdate:
	trapa	#__NR_kvram_display
	rts
	nop
.end
