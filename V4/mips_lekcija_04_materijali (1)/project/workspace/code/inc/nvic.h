#ifndef _NVIC_H_
#define _NVIC_H_

#include <stdint.h>

#include "utility.h"

/*
 * Izvor informacija:
 * 01_STM32F103_Cortex_M3_Programming_Manual -> 4.3 Nested vectored interrupt controller (NVIC) (pg. 118)
 *
 */

typedef struct
{
	uint32_t ISER[3];
	uint32_t dummy0[29];
	uint32_t ICER[3];
	uint32_t dummy1[29];
	uint32_t ISPR[3];
	uint32_t dummy2[29];
	uint32_t ICPR[3];
	uint32_t dummy3[29];
	uint32_t IABR[3];
	uint32_t dummy4[61];
	uint32_t IPR[21];
} NVIC_RegisterMapType;

#define NVIC ((NVIC_RegisterMapType *) 0xE000E100)

#define NVIC_ENABLE_IRQ(irqNumber) \
	NVIC->ISER[(irqNumber) / 32] = 1 << ((irqNumber) % 32)

#define NVIC_DISABLE_IRQ(irqNumber) \
	NVIC->ICER[(irqNumber) / 32] = 1 << ((irqNumber) % 32)

#define NVIC_SET_PRIORITY(irqNumber, priority) \
	SET_VALUE( \
		NVIC->IPR[(irqNumber) / 4], \
		((irqNumber) % 4) * 8, \
		((irqNumber) % 4) * 8 + 7, \
		(priority))

#endif
