.cpu cortext-m3
.fpu softvfp
.syntax unified
.thumb

.extern _main_stack_pointer_value

.weak systick_handler
.thumb_set systick_handler, default_handler

.section .vektor_table, "a"
.word _main_stack_pointer_value
.word reset_handler
.rept 13
	.word .default_handler
.endr
.word systick_handler
.rept 68
	.word .default_handler
.endr

.extern _vma_data_start
.extern _vma_data_end
.extern _lma_data_start

.section .text.reset_handler
.type reset_handler, %function
reset_handler:
	ldr r0, =_lma_data_start
	ldr r1, =_vma_data_start
	ldr r2, =_vma_data_end

	cmp r1, r2
	beq branch_to_main

copy_loop:
	ldr r3, [r0], 4
	str r3, [r1], 4
	cmp r1, r2
	blo copy_loop

branch_to_main:
	b main
infinite_loop:
	b infinite_loop


.section .text.default_handler
.type default_handler, %function
default_handler:
	b default_handler
.end
