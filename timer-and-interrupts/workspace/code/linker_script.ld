_main_stack_pointer_value = 0x20002800;

MEMORY
{
	FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 32K
	SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 10K
}

SECTIONS
{
	.vector_table :
	{
		*(.vector_table)
	} > FLASH
	
	.text :
	{
		*(.text)
		*(.text*)
	} > FLASH

	.data :
	{
		_vma_data_start = .;
		*(.data)
		*(.data*)
		_vma_data_end = .;
	} > SRAM AT> FLASH
	
	_lma_data_start = LOADADDR(.data);
	
	.rodata :
	{
		*(.rodata)
		*(.rodata*)
	} > FLASH

	.bss :
	{
		*(.bss)
		*(.bss*)
		*(COMMON)
	} > SRAM
	
}