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

int main()
{

	bss = 0;

	while(1)
	{

	}
	return 0;
}
