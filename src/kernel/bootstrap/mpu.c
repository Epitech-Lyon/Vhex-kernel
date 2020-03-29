#include <asm/types.h>

/* check_sh3 - Detecting sh3-based MPU */
static mpu_t check_sh3(uint16_t tplcr)
{
	if (tplcr == 0x0fff)
		return (MPU_SH7337);
	if (tplcr == 0x00ff)
		return (MPU_SH7355);
	return (MPU_UNKNOWN);
}

/* check_sh3 - Detecting sh4-based MPU */
static mpu_t check_sh4(void)
{
	volatile uint32_t *pvr	= (void *)0xff000030;
	volatile uint32_t *prr	= (void *)0xff000044;
	uint32_t product;

	if ((*pvr & 0xffffff00) != 0x10300b00)
		return (MPU_UNKNOWN);
	product = *prr & 0xfffffff0;
	if (product == 0x00002c00)
		return (MPU_SH7305);
	if (product == 0x00002200)
		return (MPU_SH7724);
	return (MPU_UNKNOWN);
}

//
// get_mpu()
//
// Return the MPU identifier of the calculator.
// Thanks to SimonLothar and Lephenixnoir for this function
// and related information.
//
// Processor version register (PVR) and product control register (PRR)
// hold information about the MPU version, they but are only accessible on
// SH-4-based MPUs.
// To detect SH-3-based MPUs, this function uses port L control register
// (PLCR), whose bits 8 to 15 cannot be set with SH7337 where bits 8 to 11
// can be set with SH7355.
//
// Additionally, the CPU core ID register (CPIDR) at 0xff000048 returns 1
// on SH7305.
//
mpu_t mpu_get(void)
{
	volatile uint16_t *plcr	= (void *)0xa4000114;
	uint16_t splcr;
	uint16_t tplcr;
	mpu_t mpu;

	// Check port L control register.
	splcr = *plcr;
	*plcr = 0xffff;
	tplcr = *plcr;
	*plcr = splcr;

	mpu = check_sh3(tplcr);
	if (mpu != MPU_UNKNOWN)
		return (mpu);
	return (check_sh4());
}
