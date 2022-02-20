#include "systick.h"

void systick_handler(){
	systick_callback();
}


void irq0_wwdg_handler(){
	// Empty
}

void irq1_pvd_handler(){
	// Empty
}

void irq2_tamper_handler(){
	// Empty
}
