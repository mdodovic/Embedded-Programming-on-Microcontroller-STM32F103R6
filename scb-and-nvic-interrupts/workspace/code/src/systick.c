#include <stdint.h>
#include "systick.h"

void systick_init()
{
	STK->LOAD = 8000000 / 2 - 1;
	STK->VAL = 0;
	STK->CTRL |= CTRL_ENABLE | CTRL_TICKINT | CTRL_CLKSOURCE;
}

void __attribute__((weak)) systick_callback()
{
	// empty
}
