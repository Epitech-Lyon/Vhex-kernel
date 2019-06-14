#include "tests/internal.h"
#include <malloc.h>
#include <stdarg.h>

// Internal prototypes / static symbols.
static void my_malloc_init_hook(void);
static void *my_malloc_hook(size_t size, const void *caller);
static void *old_malloc_hook;
static int hook_dyn_fail;
static int hook_status;

//
// my_malloc_hook()
// This function will replace the current malloc hook by our own
// hook (which can return NULL if we want, it allows us to check
// the behavior of our code).
//
// NOTE:
// "man __malloc_hook" indicate to use "__malloc_initialize_hook"
// which will execute the initialization routine. But from glibc 2.24
// onwards, this variable has been removed from the API.
// So, this is why we use "constructor" attribute to call our routine
// at the beginning of the test.
//
__attribute__((constructor))
static void my_malloc_init_hook(void)
{
	old_malloc_hook = __malloc_hook;
	__malloc_hook = my_malloc_hook;
	hook_status = FAIL_NONE;
}

//
// my_malloc_hook()
// For some tests we need to simulate (properly) a malloc fail; to do this
// we use a malloc hook and add static variables to force him to return
// NULL address.
//
// NOTE:
// All hooks of the glibc will be removed because of their poor behavior
// in multi threaded environment.
// But recently we have seen that the malloc() function is defined by the
// weak symbol, so it is probably possible to override malloc() and do
// the same job.
//
static void *my_malloc_hook(size_t size, const void *caller)
{
	void *result;

	if (hook_status != FAIL_NONE && hook_status != FAIL_NEXT_DYN){
		if (hook_status == FAIL_NEXT)
			hook_status = FAIL_NONE;
		return (NULL);
	}
	if (hook_status == FAIL_NEXT_DYN){
		hook_dyn_fail -= 1;
		if (hook_dyn_fail <= 0){
			hook_status = FAIL_NONE;
			return (NULL);
		}
	}
	__malloc_hook = old_malloc_hook;
	result = malloc(size);
	(void)caller;
	//printf("caller: %p - area: %p\n", caller, result);
	old_malloc_hook = __malloc_hook;
	__malloc_hook = my_malloc_hook;
	return (result);
}

//
// malloc_hook_udpate()
// This function is the only way to interact with the hook; it will
// define the behavior of malloc:
//	* FAIL_NEXT	: force the next call of malloc to return NULL.
//	* FAIL_NEXT_DYN	: force X-nth call of malloc to return NULL.
//	* FAIL_ALWAYS	: force all malloc return NULL.
//	* FAIL_NONE	: force malloc to work normally.
//
void malloc_hook_update(status_t status, ...)
{
	va_list ap;

	hook_dyn_fail = 0;
	if (status == FAIL_NEXT_DYN){
		va_start(ap, status);
		hook_dyn_fail = va_arg(ap, int);
		va_end(ap);
	}
	hook_status = status;
}
