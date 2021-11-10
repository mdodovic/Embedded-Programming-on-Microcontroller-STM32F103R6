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

#define SCB ((SCB_Register_Map*) (0xE000ED00))

// Usage, Bus and Memory are prioritized by SHPR1 register
#define SET_USAGE_FAULT_PRIORITY(priority) OR_VALUE(SCB->SHPR1, 20, 23, priority)
#define SET_BUS_FAULT_PRIORITY(priority) OR_VALUE(SCB->SHPR1, 12, 15, priority)
#define SET_MEMORY_MANAGEMENT_FAULT_PRIORITY(priority) OR_VALUE(SCB->SHPR1, 4, 7, priority)

// SVCall is prioritized by SHPR2 register
#define SET_SVCALL_PRIORITY(priority) OR_VALUE(SCB->SHPR2, 28, 31, priority)

// SysTick and PendSV are prioritized by SHPR3 register
#define SET_SYSTICK_PRIORITY(priority) OR_VALUE(SCB->SHPR3, 28, 31, priority)
#define SET_PENDSV_PRIORITY(priority) OR_VALUE(SCB->SHPR3, 20, 23, priority)

#define SET_AIRCR_PRIORITY_GROUP(priority_group) SET_VALUE(SCB->AIRCR, 8, 10, priority_group)

#endif // _SCB_H_
