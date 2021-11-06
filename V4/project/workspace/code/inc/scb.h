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
	uint32_t dummy0[1];
	uint32_t MMAR;
	uint32_t BFAR;

} SCB_RegisterMapType;

#define SCB ((SCB_RegisterMapType *) (0xE000ED00))

#define SCB_ICSR_NMIPENDSET         (1 << 31)
#define SCB_ICSR_PENDSVSET          (1 << 28)

#define SCB_AIRCR_PRIGROUP(prigroup) SET_VALUE(SCB->AIRCR, 8, 10, (prigroup))


#define SCB_CCR_UNALIGN_TRP 		(1 << 3)
#define SCB_CCR_DIV_0_TRP           (1 << 4)

#define SCB_SHCSR_USGFAULTENA       (1 << 18)


#endif //_SCB_H_
