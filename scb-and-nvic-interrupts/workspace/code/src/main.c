#include <stdint.h>
#include "systick.h"
#include "scb.h"
#include "nvic.h"


uint8_t const rodata[] = "VMA:FLASH,LMA:FLASH";

uint8_t data[] = "VMA:SRAM,LMA:FLASH";

uint8_t bss;

void systick_callback()
{
	bss++;
}

int main()
{

	bss = 0;

	systick_init();

	// divide by zero
	SCB->CCR |= SCB_CCR_DIV_0_TRP;
	SCB->SHCRS |= SCB_SHCRS_DIV_0_TRP;

	uint32_t dummy1 = 1;
	uint32_t zero = 0;
	uint32_t dummy2 = dummy1 / zero;


	while(1)
	{

	}
	return 0;
}
