#include <kernel/process.h>
#include <kernel/util/atomic.h>
#include <lib/pthread.h>
#include <lib/unistd.h>

int pthread_mutex_init(struct __pthread_mutex_s *mutex,
				const struct __pthread_mutexattr_s *attr)
{
	// FIXME: handle attribute
	(void)attr;

	// Initialize mutex
	mutex->lock = 0;
	return (0);
}

int pthread_mutex_lock(struct __pthread_mutex_s *mutex)
{
	// Wait util the mutex is unlocked
	while (1)
	{
		// Check if the mutex is unlock
		atomic_start();
		if (mutex->lock == 0)
			break;
		atomic_stop();

		// TODO: schedule
		__asm__ volatile ("sleep");
	}

	// Lock the mutex
	mutex->lock = 1;

	// Stop atomic operations
	atomic_stop();
	return (0);
}

int pthread_mutex_unlock(struct __pthread_mutex_s *mutex)
{
	// Check validity and unlock mutex if possible
	atomic_start();
	mutex->lock = 0;
	atomic_stop();
	return (0);
}

//TODO
int pthread_mutex_destroy(struct __pthread_mutex_s *mutex)
{
	(void)mutex;
	return (-1);
}
