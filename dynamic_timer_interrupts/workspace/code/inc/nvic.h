#ifndef _NVIC_H_
#define _NVIC_H_

#include <stdint.h>
#include "utility.h"

typedef struct
{
	uint32_t ISER[3];
	uint32_t dummy1[29];

	uint32_t ICER[3];
	uint32_t dummy2[29];

	uint32_t ISPR[3];
	uint32_t dummy3[29];

	uint32_t ICPR[3];
	uint32_t dummy4[29];

	uint32_t IABR[3];
	uint32_t dummy5[61];

	uint32_t IPR[0];

} NVIC_Register_Map;

#define NVIC ((NVIC_Register_Map*)(0xE000E100))

#define NVIC_ENABLE_IRQ(irq_number)\
	NVIC->ISER[irq_number / 32] |= 1 << (irq_number % 32)

#define NVIC_DISABLE_IRQ(irq_number)\
	NVIC->ICER[irq_number / 32] |= 1 << (irq_number % 32)

#define NVIC_SET_PRIORITY_IRQ(irq_number, priority)\
	SET_VALUE(NVIC->IPR[irq_number / 4], (irq_number % 4) * 8, (irq_number % 4) * 8 + 7, priority)


#endif // _NVIC_H_
