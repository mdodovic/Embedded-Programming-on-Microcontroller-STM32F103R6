#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>

/*
 * Izvor informacija:
 * 01_STM32F103_Cortex_M3_Programming_Manual -> 4.5 SysTick timer (STK) (pg. 150)
 *
 */

typedef struct
{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;
} STK_RegisterMapType;

#define STK ((STK_RegisterMapType *) 0xE000E010)

#define STK_CTRL_ENABLE             (1 << 0)
#define STK_CTRL_TICKINT            (1 << 1)
#define STK_CTRL_CLKSOURCE          (1 << 2)
#define STK_CTRL_COUNTFLAG          (1 << 16)

extern void systick_init();
extern void systick_callback();

#endif
