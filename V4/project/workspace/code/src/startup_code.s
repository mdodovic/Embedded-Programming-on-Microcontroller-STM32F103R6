.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb

.extern _main_stack_pointer_value

.weak nmi_handler
.thumb_set nmi_handler, default_handler

.weak hard_fault_handler
.thumb_set hard_fault_handler, default_handler

.weak mem_fault_handler
.thumb_set mem_fault_handler, default_handler

.weak bus_fault_handler
.thumb_set bus_fault_handler, default_handler

.weak usage_fault_handler
.thumb_set usage_fault_handler, default_handler

.weak sv_call_handler
.thumb_set sv_call_handler, default_handler

.weak pend_sv_call_handler
.thumb_set pend_sv_call_handler, default_handler

.weak systick_handler
.thumb_set systick_handler, default_handler

.weak irq0_WWD
.thumb_set irq0_WWD, default_handler

.weak irq1_PVD
.thumb_set irq1_PVD, default_handler

.weak irq2_TAMPER
.thumb_set irq2_TAMPER, default_handler

.weak irq3_RTC
.thumb_set irq3_RTC, default_handler

.section .vector_table, "a"
.word _main_stack_pointer_value
.word reset_handler
.word nmi_handler
.word hard_fault_handler
.word mem_fault_handler
.word bus_fault_handler
.word usage_fault_handler
.rept 4
	.word default_handler
.endr
.word sv_call_handler
.rept 2
	.word default_handler
.endr
.word pend_sv_call_handler
.word systick_handler
.word irq0_WWD
.word irq1_PVD
.word irq2_TAMPER
.word irq3_RTC
.rept 64
	.word default_handler
.endr

.extern main
# uvoz simbola definisanih u okviru linkerske skripte
.extern _lma_data_start
.extern _vma_data_start
.extern _vma_data_end
.extern instruction_set
.section .text.reset_handler
.type reset_handler, %function
reset_handler:
	ldr r0, =_lma_data_start
	ldr r1, =_vma_data_start
	ldr r2, =_vma_data_end
	# provera da li postoje podaci sa inicijalnim vrednostima
	cmp r1, r2
	beq branch_to_main
	# kopiranje, rec po rec, inicijalnog sadrzaja .data sekcije
	# sa njene LMA adrese (adresa na koju je sadrzaj sekcije fizicki smesten)
	# na njenu VMA adresu (adresa na koju je sadrzaj sekcije logicki mapiran)
copy_loop:
	ldr r3, [r0], #4
	str r3, [r1], #4
	cmp r1, r2
	blo copy_loop
	# odmah je moguce preneti kontrolu toka na glavni program u okviru main funkcije zato sto:
	#
	# 1) sekcija .rodata je sekcija za nepromenljive podatka cije se vrednosti ne menjaju usled cega
	# ne postoji potreba da se vrsi kopiranje (mogu ostati u FLASH memoriji)
	#
	# 2) sekcija .bss je sekcija za podatke bez incijalnih vrednost usled cega
	# ne postoji potreba da se vrsi kopiranje (ne postoji inicijalni sadrzaj)
branch_to_main:
	bl instruction_set
	b main
infinite_loop_1:
	b infinite_loop_1

.section .text.default_handler
.type default_handler, %function
default_handler:
infinite_loop_2:
	b infinite_loop_2

	.end
