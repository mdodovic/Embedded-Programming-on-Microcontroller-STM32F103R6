#ifndef _SCB_H_
#define _SCB_H_

#include <stdint.h>
#include "utility.h"

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
	uint32_t SHCSR;
	uint32_t CFSR;
	uint32_t HFSR;

	uint32_t dummy_30_34[1];

	uint32_t MMAR;
	uint32_t BFAR;

} SCB_Register_Map;

#define SCB ((SCB_Register_Map*)(0xE000ED00))

#define SCB_AIRCR_SET_PATTERN(priority_pattern) \
	SET_VALUE(SCB->AIRCR, 8, 10, priority_pattern)

#define SCB_ICSR_PENDSV_SET (1 << 28)
#define SCB_SHCSR_SVCALLPENDED (1 << 15)


#endif //_SCB_H_



