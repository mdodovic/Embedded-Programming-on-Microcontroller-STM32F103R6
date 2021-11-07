#include "systick.h"
#include "scb.h"
#include "nvic.h"

void systick_handler()
{
	systick_callback();
}


void nmi_handler()
{
	// Empty
}

void hard_fault_handler()
{
	// Empty
}

void memory_manage_handler()
{
	// Empty
}

void bus_fault_handler()
{
	// Empty
}

void usage_fault_handler()
{
	// Empty
}

void svcall_handler()
{
	// Empty
}

void pendsv_handler()
{
	// Empty
}

void irq0_wwdg_handler()
{
	// Empty
}

void irq1_pvd_handler()
{
	// Empty
}

void irq2_tamper_handler()
{
	// Empty
}

void irq3_rtc_handler()
{
	// Empty
}
