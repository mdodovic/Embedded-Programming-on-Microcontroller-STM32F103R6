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

extern uint32_t number_of_current_interrupt();

uint32_t current_interrupt = 0;

int main()
{
	SET_PRIORITY_PATTERN(0x5); // xx.yy

	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);

	NVIC_SET_PRIORITY(0, 0x30); // 00.11
	NVIC_SET_PRIORITY(1, 0x40); // 01.00
	NVIC_SET_PRIORITY(2, 0x50); // 01.01

	// basepri <= 40 : all GROUP priorities equal to 40 or higher will be blocked
	__asm__(
		"mov r0, 0x40\n"
		"msr basepri, r0"
	);

	NVIC->ISPR[0] |= 0x7;

	SCB_SET_SYSTICK_PRIORITY(0x00);
	SCB_SET_PENDSV_PRIORITY(0x10); // 00.01
	SCB_SET_SVCALL_PRIORITY(0x20);

	SCB->ICSR |= SCB_ICSR_PENDSVSET;
	__asm__(
		"svc 0"
	);

	current_interrupt = number_of_current_interrupt();

	bss = 0;

	systick_init();

	while(1)
	{

	}
	return 0;
}
