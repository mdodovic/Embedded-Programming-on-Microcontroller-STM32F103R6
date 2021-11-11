_main_stack_pointer_value = 0x20002800;

MEMORY
{
    FLASH (rw) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 10K
}

SECTIONS
{
  .vector_table :
  {
	*(.vector_table)
  } >FLASH
  
  .text :
  {
	*(.text)
	*(.text*)
  } >FLASH

  .rodata :
  {
	*(.rodata)
	*(.rodata*)
  } >FLASH

  .data :
  {
	*(.data)
	*(.data*)
  } >SRAM AT>FLASH

  .bss :
  {
	*(.bss)
	*(.bss*)
	*(COMMON)
  } >SRAM

}
