#include <stdint.h>

#include "systick.h"
#include "scb.h"
#include "nvic.h"

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:RAM, LMA:FLASH";

uint32_t bss; // "VMA:RAM, LMA:(not loaded)"

uint32_t main()
{

	SCB->CCR |= SCB_CCR_DIV_0_TRP;
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA;

	uint32_t dummy1 = 1, zero = 0;
	uint32_t dummy2 = dummy1 / zero; // Nakon ove linije sledi izvrsavanje prekidnih rutina, opisano preko slika u mejlu.


	// Nastavak vezbi 4, koji se ne odnosi na pitanje.
	SCB->CCR |= SCB_CCR_UNALIGN_TRP;

	uint32_t unaligned_word_address1 = 0x08000005;
	uint32_t unaligned_word_address2 = 0x08000005;


	__asm__(
				"mov r0, %[address]\n"
				"ldr r1, [r0]\n"
				"ldm r0!, {r1-r3}"
				: [address] "=r" (unaligned_word_address1)
		);

	uint32_t dummy3 = (* ((uint32_t *) unaligned_word_address2));

	SCB->ICSR |= SCB_ICSR_NMIPENDSET;
	SCB->ICSR |= SCB_ICSR_PENDSVSET;

	SCB->ICSR |= SCB_ICSR_NMIPENDSET | SCB_ICSR_PENDSVSET;

	__asm__(
				"mov r0, 1 \n"
				"msr faultmask, r0"
		);

	SCB->ICSR |= SCB_ICSR_NMIPENDSET | SCB_ICSR_PENDSVSET;
	__asm__(
				"mov r0, 0 \n"
				"msr faultmask, r0"
		);

	__asm__(
				"svc 0"
		);




	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);
	NVIC_ENABLE_IRQ(3);
	NVIC->ISPR[0] |= 0x0F;


	SCB_AIRCR_PRIGROUP(0x3);
	NVIC_SET_PRIORITY(0, 0x70);
	NVIC_SET_PRIORITY(1, 0x60);
	NVIC_SET_PRIORITY(2, 0x50);
	NVIC_SET_PRIORITY(3, 0x40);

	NVIC->ISPR[0] |= 0x0F;

	__asm(
			"mov r0, 0x30 \n"
			"msr basepri, r0"
	);

	NVIC->ISPR[0] |= 0x0F;
	__asm(
				"mov r0, 0x60 \n"
				"msr basepri, r0"
		);

	NVIC->ICPR[0] |= 0x0F;

	SCB_AIRCR_PRIGROUP(5);
	NVIC->ISPR[0] |= 0x0F;
	SCB_AIRCR_PRIGROUP(4);

	__asm(
		"mov r0, 0x01\n"
		"msr control, r0"
	);


	__asm__(
					"svc 0"
			);

	systick_init();

	while (1)
	{
		/*
		 * Beskonacna petlja kojom se sprecava povratak u reset_handler
		 * pri cemu se reaguje na zahteve za obradu izuzetka.
		 *
		 */
	}

	return 0;
}
