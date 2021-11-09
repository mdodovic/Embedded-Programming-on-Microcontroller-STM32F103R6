#ifndef _SCB_H_
#define _SCB_H_

#include <stdint.h>
#include "utilities.h"

typedef struct
{
	uint32_t CPUID;
	uint32_t ICSR;
	uint32_t VTOR;
	uint32_t AIRCR;
	uint32_t SCR;
	uint32_t CCR;
	uint32_t SHPR1;
	uint32_t SHPR2;
	uint32_t SHPR3;
	uint32_t SHCRS;
	uint32_t CFSR;
	uint32_t HFSR;

	uint32_t dummy_30_34[1];

	uint32_t MMAR;
	uint32_t BFAR;

} SCB_Register_Map;

#define SCB ((SCB_Register_Map*)(0xE000ED00))

#define SCB_ICSR_NMIPENDSET (1 << 31)
#define SCB_ICSR_PENDSVSET (1 << 28)
#define SCB_ICSR_PENDSVCLR (1 << 27)

#define SCB_AIRCR_PRIGROUP(priority_group) SET_VALUE(SCB->AIRCR, 8, 10, (priority_group))

#define SCB_CCR_DIV_0_TRP (1 << 4)
#define SCB_CCR_UNALIGN_TRP (1 << 3)


#define SCB_SHCRS_USAGEFAULTENA (1 << 18)


#endif //_SCB_H_



