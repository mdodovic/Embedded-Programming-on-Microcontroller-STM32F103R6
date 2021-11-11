#include <stdint.h>

#include "scb.h"
#include "systick.h"
#include "nvic.h"

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:SRAM, LMA:FLASH";
uint32_t bss;

int main()
{

	bss = 0;

	SET_AIRCR_PRIORITY_GROUP(0x5);

	SET_PEND_SV_PRIORITY(0xF0);

	SCB->ICSR |= SCB_ICSR_PEND_PEND_SV;

	while(1)
	{

	}
	return 0;
}
