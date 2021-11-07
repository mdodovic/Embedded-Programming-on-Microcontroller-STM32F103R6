#include <stdint.h>

#include "systick.h"
#include "scb.h"
#include "nvic.h"

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:RAM, LMA:FLASH";
uint32_t bss; // "VMA:RAM, LMA:(not loaded)"

uint32_t main()
{
	// Deljenje nulom ----------------------------------------------------------
	SCB->CCR |= SCB_CCR_DIV_0_TRP;
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA;

	uint32_t dummy1 = 1, zero = 0;
//	// Usage fault (DIVBYZERO)
//	uint32_t dummy2 = dummy1 / zero;

	// Unaligned pristup memoriji ----------------------------------------------
	SCB->CCR |= SCB_CCR_UNALIGN_TRP;

	uint32_t unaligned_word_address1 = 0x08000005;
	uint32_t unaligned_word_address2 = 0x20000007;
	// Usage fault (UNALIGNED) ??? (simulator nije idealan)
	__asm__(
			"mov r0, %[address]\n"
			"ldr r1, [r0]\n"
			"ldm r0!, {r1-r3}"
			: [address] "=r" (unaligned_word_address1)
	);
	// Usage fault (UNALIGNED) ??? (simulator nije idealan)
	uint32_t dummy3 = (*((uint32_t*) unaligned_word_address2));

	// Postavljanje pending bita za NMI i PendSV izuzetke ----------------------
	SCB->ICSR |= SCB_ICSR_NMIPENDSET | SCB_ICSR_PENDSVSET;

	// FAULTMASK ---------------------------------------------------------------
	__asm__(
			"mov r0, 1\n"
			"msr faultmask, r0"
	);
	SCB->ICSR |= SCB_ICSR_NMIPENDSET | SCB_ICSR_PENDSVSET;
	__asm__(
			"mov r0, 0\n"
			"msr faultmask, r0"
	);

	// SVC ---------------------------------------------------------------------
	__asm__("svc 0");

	// Omogucavanje i postavljanje pending bita za prekide ---------------------
	NVIC_ENABLE_IRQ(0);
	NVIC_ENABLE_IRQ(1);
	NVIC_ENABLE_IRQ(2);
	NVIC_ENABLE_IRQ(3);
	NVIC->ISPR[0] |= 0x0F; // postavljanje pending bita za IRQ3, IRQ2, IRQ1, IRQ0

	// Postavljanje prioriteta za prekide --------------------------------------
	SCB_AIRCR_PRIGROUP(3);
	NVIC_SET_PRIORITY(0, 0x70);
	NVIC_SET_PRIORITY(1, 0x60);
	NVIC_SET_PRIORITY(2, 0x50);
	NVIC_SET_PRIORITY(3, 0x40);
	NVIC->ISPR[0] |= 0x0F; // postavljanje pending bita za IRQ3, IRQ2, IRQ1, IRQ0

	// BASEPRI -----------------------------------------------------------------
	__asm__(
			"mov r0, 0x30\n"
			"msr basepri, r0"
	);
	NVIC->ISPR[0] |= 0x0F; // postavljanje pending bita za IRQ3, IRQ2, IRQ1, IRQ0
	__asm__(
			"mov r0, 0x60\n"
			"msr basepri, r0"
	);

	// PRIGROUP ----------------------------------------------------------------
	NVIC->ICPR[0] |= 0x0F; // brisanje pending bita za IRQ3, IRQ2, IRQ1, IRQ0
	SCB_AIRCR_PRIGROUP(5);
	NVIC->ISPR[0] |= 0x0F; // postavljanje pending bita za IRQ3, IRQ2, IRQ1, IRQ0
	SCB_AIRCR_PRIGROUP(4);

	// Unprivileged i Privileged nivoi izvrsavanja -----------------------------
	__asm__(
			"mov r0, 0x1\n"
			"msr control, r0\n"
	);
	// Bezuspesan pokusaj povratka u Unprivileged nivo izvrsavanja
	__asm__(
			"mov r0, 0x0\n"
			"msr control, r0\n"
	);

	__asm__ ("svc 0x00");

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
