#include <stdint.h>
#include "utility.h"
#include "systick.h"

void __attribute__((weak)) systick_callback()
{
	// Empty
}

void systick_init()
{
	STK->LOAD = 8000000 / 2 - 1;
	STK->VAL = 0;
	STK->CTRL |= STK_CTRL_ENABLE | STK_CTRL_TICKINT | STK_CTRL_CLKSOURCE;

}

uint32_t flag = 0;

void systick_handler()
{

	if(flag > 0)
	{
		STK->CTRL &= ~STK_CTRL_ENABLE; // disable

		if(flag % 2 == 1)
		{
			STK->LOAD = 8000000 - 1;
		}
		else
		{
			STK->LOAD = 8000000 / 2 - 1;
		}

		STK->CTRL |= STK_CTRL_ENABLE; // enable
	}
	flag ++;
	systick_callback();
}
