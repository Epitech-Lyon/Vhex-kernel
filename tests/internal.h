#ifndef __INTERNAL_H__
# define __INTERNAL_H__

#include <stddef.h>
#include <stdint.h>

//
// status_t - all status available for hook.
//	* FAIL_NONE	: force malloc to work normaly.
//	* FAIL_NEXT	: force the next call of malloc to return NULL.
//	* FAIL_NEXT_DYN	: force X-nth call of malloc to return NULL.
//	* FAIL_ALWAYS	: force all malloc to return NULL.
//
typedef enum status_e
{
	FAIL_NONE,
	FAIL_NEXT,
	FAIL_ALWAYS,
	FAIL_NEXT_DYN
} status_t;

/* malloc_hook_update - udpate the behavior of the malloc hook */
void malloc_hook_update(status_t status, ...);

#endif /*__INTERNAL_H__*/
