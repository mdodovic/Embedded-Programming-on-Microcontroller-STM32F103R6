#include <stdint.h>

uint8_t const rodata[] = "VMA:FLASH, LMA: FLASH";
uint8_t data[] = "VMA:SRAM, LMA: FLASH";
uint32_t bss;

int main()
{

	bss = 0;

	while(1)
	{

	}
	return 0;
}
