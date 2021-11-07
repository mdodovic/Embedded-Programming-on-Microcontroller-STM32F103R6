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
	SCB->SHCRS |= SCB_SHCRS_USAGEFAULTENA;

	uint32_t dummy1 = 1;
	uint32_t zero = 0;
//	uint32_t dummy2 = dummy1 / zero;

	// unaligned access
	SCB->CCR |= SCB_CCR_UNALIGN_TRP;

	uint32_t unalign_address = 0x80000001;
//	uint32_t unalign_address_access = (*((uint32_t*)(unalign_address)));

//	__asm__(
//			"mov r0, %[address]\n"
//			"ldr r1, [r0]\n"
//			"ldm r0!, {r1-r3}\n"
//			: [address] "=r" (unalign_address)
//	);

	// NMI, PendSV, FAULTMASK
	// pending NMI
	SCB->ICSR |= SCB_ICSR_NMIPENDSET;
	__asm(
			"nop\n"
	);
	// pending PendSV
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
	__asm(
			"nop\n"
	);
	// pending NMI and PendSV simultaneously
	SCB->ICSR |= SCB_ICSR_PENDSVSET | SCB_ICSR_NMIPENDSET;
	__asm(
			"nop\n"
	);

	// FAULTMASK register (mask all except NMI)
	__asm(
			"mov r0, 1\n"
			"msr faultmask, r0"
	);
	SCB->ICSR |= SCB_ICSR_PENDSVSET | SCB_ICSR_NMIPENDSET;
	__asm(
			"nop\n"
	);
	__asm(
			"mov r0, 0\n"
			"msr faultmask, r0"
	);

	// pending NMI and PendSV simultaneously and then clear PendSV
	__asm(
			"mov r0, 1\n"
			"msr faultmask, r0"
	);
	SCB->ICSR |= SCB_ICSR_PENDSVSET | SCB_ICSR_NMIPENDSET;
	__asm(
			"nop\n"
	);
	SCB->ICSR |= SCB_ICSR_PENDSVCLR;
	__asm(
			"mov r0, 0\n"
			"msr faultmask, r0"
	);

	// SVCall
	__asm(
			"svc 0"
	);
	// pending NMI and PendSV and call SVCall simultaneously
	__asm(
			"mov r0, 1\n"
			"msr faultmask, r0"
	);
	SCB->ICSR |= SCB_ICSR_PENDSVSET | SCB_ICSR_NMIPENDSET;
	__asm(
			"svc 0"
	);
	__asm(
			"nop\n"
	);
	__asm(
			"mov r0, 0\n"
			"msr faultmask, r0"
	);
	__asm(
			"nop\n"
	);



	while(1)
	{

	}
	return 0;
}
