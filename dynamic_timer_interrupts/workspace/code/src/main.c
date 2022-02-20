#include <stdint.h>

#include "scb.h"
#include "systick.h"
#include "nvic.h"

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:SRAM, LMA:FLASH";
uint32_t bss;

void systick_callback()
{
	bss++;
}

int main()
{

	bss = 0;

	SET_AIRCR_PRIORITY_GROUP(0x5);

	SET_PEND_SV_PRIORITY(0xF0);

	SCB->ICSR |= SCB_ICSR_PEND_PEND_SV;


	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);
								// xx.yy
	NVIC_SET_PRIORITY_IRQ(0, 0x20); // 00.10
	NVIC_SET_PRIORITY_IRQ(1, 0x00); // 00.00
	NVIC_SET_PRIORITY_IRQ(2, 0x10); // 00.01

	NVIC->ISPR[0] |= 0x7;

	systick_init();

	while(1)
	{

	}
	return 0;
}
