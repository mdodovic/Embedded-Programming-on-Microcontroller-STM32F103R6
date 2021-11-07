#include <stdint.h>
#include "systick.h"


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

	while(1)
	{

	}
	return 0;
}
