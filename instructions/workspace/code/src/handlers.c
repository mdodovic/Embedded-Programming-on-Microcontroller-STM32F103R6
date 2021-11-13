#include <stdint.h>
#include "systick.h"

void systick_handler();

void systick_init()
{
	STK->VAL = 0;
	STK->CTRL |= STK_CTRL_TICKINT | STK_CTRL_CLKSOURCE;
	systick_handler();
}


uint8_t flag = 0;
void systick_handler()
{
	STK->CTRL &= ~STK_CTRL_ENABLE;
	if(flag)
	{
		STK->LOAD = 8000000 - 1;
	}
	else
	{
		STK->LOAD = 8000000 / 2- 1;
	}
	flag = ~flag;
	systick_callback();
	STK->CTRL |= STK_CTRL_ENABLE;
}


