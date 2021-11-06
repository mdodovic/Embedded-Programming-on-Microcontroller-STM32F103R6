#include <stdint.h>

#include "systic.h"

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:RAM, LMA:FLASH";

uint32_t bss; // VMA:RAM, LMA:(ne ucitava se)

void systic_callback()
{
	++bss;
}

uint32_t main()
{
	bss = 0;

	systick_init();

	while(1);

	return 0;
}
