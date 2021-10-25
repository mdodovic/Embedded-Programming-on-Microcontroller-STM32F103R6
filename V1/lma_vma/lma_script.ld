MEMORY
{
	REGION_PRVI(rwx)  : ORIGIN = 0x22220000, LENGTH = 32k
	REGION_DRUGI(rwx) : ORIGIN = 0x44444444, LENGTH = 32k
	REGION_TRECI(rwx) : ORIGIN = 0x88887777, LENGTH = 32k
}

SECTIONS
{
	i_s_program :
	{
		*(.text*)
	} > REGION_PRVI
	i_s_podaci :
	{
		start_vma_2 = .;
		*(.data*)
	} > REGION_DRUGI AT> REGION_TRECI
	
	i_s_podaci_start_vma_1 = ADDR(i_s_podaci);
	i_s_podaci_start_lma = LOADADDR(i_s_podaci);
	
	
}