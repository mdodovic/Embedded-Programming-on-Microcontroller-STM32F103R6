#include <stdint.h>
#include "systick.h"

void pendsv_handler()
{
	// Empty
}

uint32_t systick_counter = 0;

void systick_init()
{
	STK->LOAD = 8000000 / 2 - 1;
	STK->VAL = 0;
	STK->CTRL |= STK_CTRL_ENABLE | STK_CTRL_TICKINT | STK_CTRL_CLKSOURCE;
}


void systick_handler()
{
	STK->CTRL &= ~STK_CTRL_ENABLE;

	if(systick_counter > 0){
		if(systick_counter % 2 == 1)
		{
			STK->LOAD = 8000000 - 1;
		} else
		{
			STK->LOAD = 8000000 / 2 - 1;
		}
	}
	systick_counter++;

	STK->CTRL |= STK_CTRL_ENABLE;
	systick_callback();
}
