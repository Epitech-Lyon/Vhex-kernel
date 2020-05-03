#include <kernel/drivers/keyboard.h>
#include <kernel/hardware/keysc.h>
#include <kernel/hardware/intc.h>

// Device internal indicator to
// avoid reentrace with the key list.
volatile uint8_t keylist_isUpdate = 0;

void sh7305_keysc_int_handler(void)
{
	static uint8_t key_frame = 0;
	int column;
	int row;

	// Block / disable KEYSC interrupt.
	SH7305_INTC.IMR5.KEYI  = 1;
	SH7305_INTC.IPRF.KEYSC = 0;

	// Update key frame.
	key_frame = key_frame + 1;

	// Scan the keyboard.
	row = 6;
	while (--row >= 0)
	{
		column = 16;
		while (--column >= 0)
		{
			if (SH7305_KEYSC.KIUDATA[row] & (0x8000 >> column))
				keycache_update(row, column, key_frame);
		}
	}

	// Clean key cache (remove unpressed key)
	keycache_clean(key_frame);

	// Indicate that the key list has been udpated.
	keylist_isUpdate = 1;

	// Clear KEYSC interrupt, really.
	SH7305_KEYSC.INTERRUPT.WORD = SH7305_KEYSC.INTERRUPT.WORD;

	// Unblock / enable KEYSC interrupt.
	SH7305_INTC.IPRF.KEYSC = 15;
	SH7305_INTC.IMCR5.KEYI = 1;
}
