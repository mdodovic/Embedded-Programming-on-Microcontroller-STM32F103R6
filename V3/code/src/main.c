#include <stdint.h>

uint8_t const rodata[] = "VMA:FLASH, LMA:FLASH";
uint8_t data[] = "VMA:RAM, LMA:FLASH";

uint32_t bss; // VMA:RAM, LMA:(ne ucitava se)

uint32_t main()
{


}
