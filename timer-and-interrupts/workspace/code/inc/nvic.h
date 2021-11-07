#ifndef _NVIC_H_
#define _NVIC_H_

#include <stdint.h>

typedef struct
{
	uint32_t ISER[3];
	uint32_t dummy_0x0C_0x80[29];

	uint32_t ICER[3];
	uint32_t dummy_0x8C_0x100[29];

	uint32_t ISPR[3];
	uint32_t dummy_0x10C_0x180[29];

	uint32_t ICPR[3];
	uint32_t dummy_0x18C_0x200[29];

	uint32_t IABR[3];
	uint32_t dummy_0x20C_0x300[61];

	uint32_t IPR[21];


} SCB_Register_Map;

#define SCB ((SCB_Register_Map*)(0xE000E100))


#endif //_NVIC_H_



