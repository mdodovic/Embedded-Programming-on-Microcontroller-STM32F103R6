#include <stdint.h>
#include "utility.h"
#include "systick.h"
#include "scb.h"
#include "nvic.h"
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


void irq0_wwdg_handler()
{
	// Empty
}
void irq1_pvd_handler()
{
	// Empty
}
void irq2_tamper_handler()
{
	// Empty
}

extern uint32_t current_interrupt;

uint32_t number_of_current_interrupt()
{
	asm (
		"mrs %0, ipsr"
	    : "=r" (current_interrupt)
	);
	return current_interrupt & 0x1FF;

}

void pendsv_handler()
{
	current_interrupt = number_of_current_interrupt();
}

void svcall_handler()
{
	current_interrupt = number_of_current_interrupt();
}

