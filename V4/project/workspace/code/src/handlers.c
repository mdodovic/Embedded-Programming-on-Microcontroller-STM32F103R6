void nmi_handler()
{
	// Empty
}

void hard_fault_handler()
{
	// Empty
}

void usage_fault_handler()
{
	// Empty
}

void sv_call_handler()
{
	__asm__(
			"mov r0, 0x00\n"
			"msr control, r0"
	);
}

void pend_sv_call_handler()
{
	// Empty
}

void systick_handler()
{
	// systick_callback();
}

void irq0_WWD()
{
	// Empty
}

void irq1_PVD()
{
	// Empty
}

void irq2_TAMPER()
{
	// Empty
}

void irq3_RTC()
{
	// Empty
}
