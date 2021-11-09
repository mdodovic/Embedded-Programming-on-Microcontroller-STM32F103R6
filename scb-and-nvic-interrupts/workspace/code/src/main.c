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

	// IRQ0-IRQ3 interrupts
	// enable IRQ0-IRQ3 interrupts
	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);
	NVIC_ENABLE_IRQ(3);

	// pending IRQ0-IRQ3 interrupts
	NVIC->ISPR[0] |= 0x0F; // 1111
	__asm(
			"nop\n"
	);
	// clear pending IRQ1 interrupt
	NVIC->ICPR[0] |= 0x01; // 0001
	__asm(
			"nop\n"
	);
	NVIC->ISPR[0] |= 0x02; // 0010
	__asm(
			"nop\n"
	);


	// pending IRQ0-IRQ3 interrupts
	NVIC->ISPR[0] |= 0x0F;
	__asm(
			"nop\n"
	);
	__asm__(
			"mov r0, %[ispr_address]\n"
			: [ispr_address] "=r" (NVIC->ISPR[0])
	);
	// clear pending for IRQ1 and IRQ3 interrupts
	NVIC->ICPR[0] |= 0x0A; // 1010
	__asm(
			"nop\n"
	);
	__asm__(
			"mov r0, %[address]\n"
			: [address] "=r" (NVIC->ISPR[0])
	);
	// pending IRQ0 interrupt
	NVIC->ISPR[0] |= 0x01;
	__asm(
			"nop\n"
	);
	__asm__(
			"mov r0, %[address]\n"
			: [address] "=r" (NVIC->ISPR[0])
	);


	// disable IRQ0 and IRQ2 interrupts
	NVIC_DISABLE_IRQ(0);
	NVIC_DISABLE_IRQ(2);

	// pending IRQ0-IRQ3 interrupts
	NVIC->ISPR[0] |= 0x0F;
	__asm(
			"nop\n"
	);
	// pending IRQ0 and IRQ2 interrupts
	NVIC->ISPR[0] |= 0x05; // 0101
	__asm(
			"nop\n"
	);

	// enable IRQ0 and IRQ2 interrupts
	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(2);
	__asm(
			"nop\n"
	);
	NVIC->ISPR[0] |= 0x0F;

	// Prioriting
	// set priority pattern to be gggg
	SCB_AIRCR_PRIGROUP(3);
	NVIC_SET_PRIORITY(0, 0x70); // 0111
	NVIC_SET_PRIORITY(1, 0x60); // 0110
	NVIC_SET_PRIORITY(2, 0x50); // 0101
	NVIC_SET_PRIORITY(3, 0x40); // 0100

	// pending IRQ0-IRQ3 interrupts (watch the order)
	NVIC->ISPR[0] |= 0x0F;

	// Set base priority - nobody will be executed
	__asm__(
			"mov r0, 0x30\n"
			"msr basepri, r0"
	);

	// pending IRQ0-IRQ3 interrupts
	NVIC->ISPR[0] |= 0x0F;

	// Set base priority - nobody will be executed
	__asm__(
			"mov r0, 0x60\n"
			"msr basepri, r0"
	);
	__asm(
			"nop\n"
	);
	// pending IRQ0-IRQ3 interrupts
	NVIC->ISPR[0] |= 0x0F;

	// set priority pattern to be ggss
	SCB_AIRCR_PRIGROUP(5);
	// basepri is 60 = 01.10
	// pending IRQ0-IRQ3 interrupts
	NVIC->ISPR[0] |= 0x0F;


	// Program mode privileged/user
	// Set mode to user
	__asm__(
			"mov r0, 0x1\n"
			"msr control, r0"
	);
	// Program mode to privileged - FAILED
	__asm__(
			"mov r0, 0x1\n"
			"msr control, r0"
	);

	// call svc to change mode
	__asm(
			"svc 0\n"
	);



	while(1)
	{

	}
	return 0;
}
