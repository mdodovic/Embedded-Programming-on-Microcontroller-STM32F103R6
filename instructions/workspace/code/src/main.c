#include <stdint.h>

#include "nvic.h"
#include "scb.h"
#include "systick.h"
#include "utility.h"

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:SRAM, LMA:FLASH";
uint8_t bss;

void systick_callback()
{
	bss++;
}

uint32_t current_exception;
extern uint32_t number_of_current_exception();

int main()
{
	SCB_AIRCR_SET_PATTERN(0x5); // xx.yy

	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);

	NVIC_SET_PRIORITY(0, 0x30); // 00.11
	NVIC_SET_PRIORITY(1, 0x40); // 01.00
	NVIC_SET_PRIORITY(2, 0x50); // 01.01

	__asm__(
		"mov r0, 0x40\n"
		"msr basepri, r0"
	);

	NVIC->ISPR[0] |= 0x7;

	bss = 0;

	SCB->ICSR |= SCB_ICSR_PENDSV_SET;
	SCB->SHCSR |= SCB_SHCSR_SVCALLPENDED;

	__asm__(
		"svc 0"
	);

	current_exception = number_of_current_exception();

	systick_init();

	while(1)
	{

	}
	return 0;
}
