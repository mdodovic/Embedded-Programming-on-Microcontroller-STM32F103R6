#ifndef _SCB_H_
#define _SCB_H_

#include <stdint.h>

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

#define SCB ((SCB_Register_Map*)(0xE000E100))


#endif //_SCB_H_



