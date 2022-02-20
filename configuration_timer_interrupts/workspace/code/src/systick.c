#include <stdint.h>
#include "systick.h"


void systick_initialize(){
	STK->LOAD = 8000000/2 - 1; // 8MHz
	STK->VAL = 0;
	STK->CTRL |= STK_CTRL_ENABLE | STK_CTRL_TICKINT | STK_CTRL_CLKSOURCE;

}

void __attribute__((weak)) systick_callback(){
	// Empty
}
