.text
.global	_dclr_str_area
.type	_dclr_str_area, @function

#include "kernel/unistd_32.h"

.align 2
_dclr_str_area:
	trapa	#__NR_kvram_clr_str_area
	rts
	nop
.end
