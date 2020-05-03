#include <stdint.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/bits/driver.h>
#include <kernel/bits/mpu.h>
#include <sys/types.h>

// Internal bootstrap funtions
extern mpu_t mpu_get(void);

//TODO: secure if no driver is found !!
void bootstrap_drivers_install(void)
{
	extern uint32_t bdrivers_section;
	extern uint32_t edrivers_section;
	struct driver *driver;
	mpu_t mpu;
	int i;

	// Start atomic operations
	earlyterm_write("Load drivers...\n");
	atomic_start();

	i = -1;
	mpu = mpu_get();
	driver = (void *)&bdrivers_section;
	while ((uint32_t)&driver[++i] < (uint32_t)&edrivers_section)
	{
		// Check driver 
		if (driver[i].arch != mpu)
			continue;
		
		// Check driver integrity
		if (driver[i].install == NULL || driver[i].uninstall == NULL) {
			earlyterm_write("* WARNING - driver integrity [%s]\n", driver[i].name);
			continue;
		}

		// Try to install the driver
		// TODO: check driver error ?
		earlyterm_write("* install driver [%s]\n", driver[i].name);
		(*driver[i].install)();
	}

	// Stop atomic operations
	atomic_stop();
}

void bootstrap_drivers_uninstall(void)
{
	extern uint32_t bdrivers_section;
	extern uint32_t edrivers_section;
	struct driver *driver;
	mpu_t mpu;
	int i;

	// Start atomic operations
	earlyterm_write("Unload drivers...\n");
	atomic_start();

	// Uninstall driver (fifo style)
	mpu = mpu_get();
	driver = (void *)&bdrivers_section;
	i = ((uint32_t)&edrivers_section - (uint32_t)&bdrivers_section) / sizeof(struct driver);
	while (--i >= 0)
	{
		// Check driver 
		if (driver[i].arch != mpu)
			continue;

		// Check driver integrity
		if (driver[i].install == NULL || driver[i].uninstall == NULL) {
			earlyterm_write("* WARNING - driver integrity [%s]\n", driver[i].name);
			continue;
		}

		// Try to uninstall the driver
		// TODO: check driver error ?
		earlyterm_write("* uninstall driver [%s]\n", driver[i].name);
		(*driver[i].uninstall)();
	}

	// Stop atomic operations
	atomic_stop();
}
