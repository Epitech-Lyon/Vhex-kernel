#include <lib/display.h>

int main(void)
{
	uint32_t counter = 0;
	while (1)
	{
		// Display data
		dclr_str_area(0, 2, 20, 1);
		dprint(0, 2, "C process: %d", counter);
		dupdate();

		// Update counter and "wait"
		counter = counter + 1;
		if (counter >= 100)
			counter = 0;
		for (int i = 0 ; i < 300000 ; ++i);
	}
}
