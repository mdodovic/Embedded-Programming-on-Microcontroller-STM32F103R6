#ifndef _NVIC_H_
#define _NVIC_H_

#include <stdint.h>

typedef struct
{
	uint32_t ISER[3];
	uint32_t dummy_00C_080[29];

	uint32_t ICER[3];
	uint32_t dummy_08C_100[29];

	uint32_t ISPR[3];
	uint32_t dummy_10C_180[29];

	uint32_t ICPR[3];
	uint32_t dummy_18C_200[29];

	uint32_t IABR0[3];
	uint32_t dummy_20C_300[61];

	uint32_t IPR[21];

} NVIC_Register_Map;

#define NVIC ((NVIC_Register_Map*)(0xE000E100))

#define NVIC_ENABLE_IRQ(number) \
	NVIC->ISER[(number) / 32] = 1 << ((number) % 32)

#define NVIC_DISABLE_IRQ(number) \
	NVIC->ICER[(number) / 32] = 1 << ((number) % 32)

#define NVIC_SET_PRIORITY(irq_number, priority) \
		SET_VALUE(NVIC->IPR[(irq_number) / 4], 8 * ((irq_number) % 4), 8 * ((irq_number) % 4) + 7, (priority))

#endif //_NVIC_H_



