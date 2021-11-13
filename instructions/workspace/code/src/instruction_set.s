.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb

.extern _vma_start_value
.global instruction_test

.section .text.instruction_set
.type instruction_set, %function
instruction_test:

	push {r0-r5, lr}



	pop {r0-r5, lr}
	bx lr


data1:
.word _vma_start_value

.end
