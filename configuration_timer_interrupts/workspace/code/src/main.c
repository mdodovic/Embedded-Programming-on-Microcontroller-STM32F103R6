#include <stdint.h>

#include "systick.h"
#include "scb.h"
#include "nvic.h"


uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:RAM, LMA:FLASH";
uint8_t bss;

void systick_callback(){
	bss++;
}


int main()
{
	bss = 0;

	SET_AIRCR_PRIORITY_GROUP(0x5); // xx.yy

	SET_USAGE_FAULT_PRIORITY(0x4); 				// 01.00
	SET_BUS_FAULT_PRIORITY(0x4);                // 01.00
	SET_MEMORY_MANAGEMENT_FAULT_PRIORITY(0x4);	// 01.00
	SET_SYSTICK_PRIORITY(0x0);					// 00.00
	SET_PENDSV_PRIORITY(0x4);					// 01.00
	SET_SVCALL_PRIORITY(0x4);					// 01.00
/*
	__asm__(
			"mov r5, %[SHPR1_register]\n"
			: [SHPR1_register] "=r" (SCB->SHPR1)
	);
	__asm__(
			"mov r6, %[SHPR2_register]\n"
			: [SHPR2_register] "=r" (SCB->SHPR2)
	);
	__asm__(
			"mov r7, %[SHPR3_register]\n"
			: [SHPR3_register] "=r" (SCB->SHPR3)
	);
*/

	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);

	//NVIC->IPR[0] = 0x00201030;

	NVIC_SET_PRIORITY(0, 0x30); // 00.11 0000
	NVIC_SET_PRIORITY(1, 0x10); // 00.01 0000
	NVIC_SET_PRIORITY(2, 0x20); // 00.10 0000


	NVIC->ISPR[0] |= 0x7;
	__asm__(
		"nop\n"
	);
	NVIC->ISPR[0] |= 0x1;

	systick_initialize();

	while(1)
	{

	}
}

