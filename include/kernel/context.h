#ifndef __KERNEL_CONTEXT_H__
# define __KERNEL_CONTEXT_H__

#include <stddef.h>
#include <stdint.h>

typedef struct fx9860_context_s
{
	struct {
		uint16_t control;
		uint16_t autofix;
		uint16_t scan_mode;
		uint16_t scan_state;
		uint16_t interrupt;
		uint16_t scan_wait;
		uint16_t scan_interval;
		uint16_t kyoutdr;
		uint16_t kyindr;
	} keyboard;
	struct {
		uint16_t ipra;
		uint16_t iprb;
		uint16_t iprc;
		uint16_t iprd;
		uint16_t ipre;
		uint16_t iprf;
		uint16_t iprg;
		uint16_t iprh;
		uint16_t ipri;
		uint16_t iprj;
		uint16_t iprk;
		uint16_t iprl;
	} intc;
	struct {
		uint32_t stbcr;
		uint32_t mstpcr0;
		uint32_t mstpcr1;
		uint32_t mstpcr2;
	} power;
	struct {
		uint8_t tstr;
		struct {
			uint32_t tcor;
			uint32_t tcnt;
			uint16_t tcr;
		} timer[3];
	} tmu;
	uint32_t vbr;
	//TODO: SR SSR SPC GBR MACL MACH
} fx9860_context_t;

typedef struct common_context_s
{
	uint32_t reg[16];
	uint32_t gbr;
	uint32_t macl;
	uint32_t mach;
	uint32_t ssr;
	uint32_t spc;
	uint32_t pr;
} common_context_t;

// Context primitive.
extern void fx9860_context_save(fx9860_context_t *context);
extern void fx9860_context_restore(fx9860_context_t *context);

#endif /*__KERNEL_CONTEXT_H__*/
