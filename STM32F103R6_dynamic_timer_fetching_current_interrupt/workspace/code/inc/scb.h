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
	uint32_t SHCRS;
	uint32_t CFSR;
	uint32_t HFSR;

	uint32_t dummy_30_34[1];

	uint32_t MMAR;
	uint32_t BFAR;

} SCB_Register_Map;

#define SCB ((SCB_Register_Map*)(0xE000ED00))

#define SET_PRIORITY_PATTERN(prigroup) \
	SET_VALUE(SCB->AIRCR, 8, 10, prigroup)

#define SCB_SET_SYSTICK_PRIORITY(priority) \
	SET_VALUE(SCB->SHPR3, 24, 31, priority)

#define SCB_SET_PENDSV_PRIORITY(priority) \
	SET_VALUE(SCB->SHPR3, 16, 23, priority)

#define SCB_SET_SVCALL_PRIORITY(priority) \
	SET_VALUE(SCB->SHPR2, 24, 31, priority)


#define SCB_ICSR_NMIPENDSET (1 << 31)
#define SCB_ICSR_PENDSVSET  (1 << 28)


#endif //_SCB_H_



