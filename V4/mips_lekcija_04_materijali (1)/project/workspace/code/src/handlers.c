#include "systick.h"

void nmi()
{

}

void hard_fault()
{

}

void mem_manage_fault()
{

}

void bus_fault()
{

}

void usage_fault()
{

}

void sv_call()
{
	__asm__(
			"mov r0, 0x0\n"
			"msr control, r0\n"
	);
}

void pend_sv()
{

}

void systick()
{
	systick_callback();
}

void irq0_WWDG()
{

}

void irq1_PVD()
{

}

void irq2_TAMPER()
{

}

void irq3_RTC()
{

}
