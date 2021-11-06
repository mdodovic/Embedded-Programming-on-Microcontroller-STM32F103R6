#include <stdint.h>

uint8_t data[] = "VMA:RAM;LMA:FLASH";
uint8_t const rodata[] = "VMA:FLASH;LMA:FLASH";
uint8_t bss = 0; // VMA:RAM;LMA:not loaded

int main(){

	bss = 0;

	while(1)
	{

	}
	return 0;
}
