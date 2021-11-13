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


void irq0_wwgd_handler()
{

}
void irq1_pvd_handler()
{

}
void irq2_tamper_handler()
{

}

extern uint32_t current_exception;

uint32_t number_of_current_exception()
{
	uint32_t current;
	__asm__(
		"mrs %0, ipsr"
		: "=r" (current)
	);
	return current & 0x1ff;
}

void svcall_handler()
{
	current_exception = number_of_current_exception();
}
void pendsv_handler()
{
	current_exception = number_of_current_exception();
}
