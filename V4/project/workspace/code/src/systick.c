#include <stdint.h>

#include "systick.h"

void systick_init()
{
	/*
	 * Izvor informacija:
	 * 01_STM32F103_Cortex_M3_Programming_Manual -> 4.5 SysTick timer (STK) (pg. 150)
	 *
	 */
	STK->LOAD = 8000000 - 1;
	STK->VAL = 0;
	STK->CTRL |= STK_CTRL_TICKINT | STK_CTRL_ENABLE;
}

/*
 * GNU GCC (6.33 Declaring Attributes of Functions)
 * https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 *
 */
void __attribute__((weak)) systick_callback()
{
	// Empty
}


