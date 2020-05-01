#ifndef __LIB_PTHREAD_H__
# define __LIB_PTHREAD_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

// Define internal Posix thread mutex
// FIXME: this is a custom implemetation
struct __pthread_mutex_s
{
	pid_t owner;
	int lock;
};
typedef struct __pthread_mutex_s pthread_mutex_t;


// Define internal Posix thread mutex attributes
struct __pthread_mutexattr_s
{
	int attr;	
};

/* Mutex part */
extern int pthread_mutex_init(struct __pthread_mutex_s *mutex, const struct __pthread_mutexattr_s *attr);
extern int pthread_mutex_lock(struct __pthread_mutex_s *mutex);
extern int pthread_mutex_unlock(struct __pthread_mutex_s *mutex);
extern int pthread_mutex_destroy(struct __pthread_mutex_s *mutex);

#endif /*__LIB_PTHREAD_H__*/
