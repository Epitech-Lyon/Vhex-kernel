#include <stdint.h>
#include <stddef.h>
extern int main(void);

//define by bootstrap.ld
extern uint32_t bbss;
extern uint32_t sbss;
extern uint32_t bdata_rom;
extern uint32_t bdata_ram;
extern uint32_t sdata;


//
// section_dump()
// copy a memory region using symbol information.
//
__attribute__((section(".pretext")))
void section_dump(uint32_t * restrict dest, uint32_t * restrict load, size_t size)
{
	size_t i;

	i = -1;
	while (++i < size >> 2)
		dest[i] = load[i];
}

//
// section_reset()
// clear a memory region using symbol information.
//
__attribute__((section(".pretext")))
void section_reset(uint32_t *bsection, size_t size)
{
	size_t i;

	i = -1;
	while (++i < size >> 2)
		bsection[i] = 0;
}

//
// initialize()
// We are currently running in the storage memory, so we should
// load data in RAM and wipe the bss section for initialize static
// and global variables.
//
__attribute__((section(".pretext.entry")))
int initialize(void)
{
	int exit;

	section_reset(&bbss, (size_t)&sbss);
	section_dump(&bdata_ram, &bdata_rom, (size_t)&sdata);
	exit = main();
	return (exit);
}
