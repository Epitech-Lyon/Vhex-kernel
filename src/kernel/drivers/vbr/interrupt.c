#include <kernel/drivers/vbr.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>

// TODO: update me
void interrupt_handler(void)
{
	extern void (*int_handler[VBR_INTERRUPT_NB])(void);
	uint32_t intevt = *(uint32_t*)0xff000028;
	int id;

	// Get the interrupt ID
	VBR_GET_INTERRUPT_CODE(id, intevt);

	// Check id validity and handler error
	if (id < 0 || id >= VBR_INTERRUPT_NB || int_handler[id] == NULL)
	{
		earlyterm_write(
			"Ho crap ! Interrupt error !\n"
			"Interrupt ID (%#x)\n"
			"Error: handler not foud !\n",
			intevt
		);
		while (1);
	}

	// Call handler and exit
	(*int_handler[id])();
}

void *vbr_interrupt_set(int intcode, void (*handler)(void))
{
	extern void (*int_handler[VBR_INTERRUPT_NB])(void);
	void *old;
	int id;

	// Get the interrupt ID
	VBR_GET_INTERRUPT_CODE(id, intcode);

	// Check id validity
	if (id < 0 || id >= VBR_INTERRUPT_NB)
		return (NULL);

	// Start atomic operations
	atomic_start();

	// Switch interupt handler
	old = int_handler[id];
	int_handler[id] = handler;

	// Stop atomic operations
	atomic_stop();

	// Return old interrupt handler
	return (old);
}
