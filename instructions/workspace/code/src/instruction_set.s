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

	adr r0, data1
	ldr r1, [r0]

	add r0, r0, 4

	ldrb r2, [r0, 1]
	ldrh r2, [r0, 1]
	ldr r2, [r0, 1]

	adr r0, data1
	ldrb r3, [r0, 1]!
	adr r0, data1
	ldrh r3, [r0, 1]!
	adr r0, data1
	ldr r3, [r0, 1]!

	adr r0, data1
	ldrb r4, [r0], 1
	adr r0, data1
	ldrh r4, [r0], 1
	adr r0, data1
	ldr r4, [r0], 1

	ldr r0, =data1
	ldr r1, [r0]

	ldr r0, =data1

	ldm r0, {r1-r4}
	ldm r0, {r1, r2, r3, r4}
	ldmia r0, {r1, r2, r3, r4}
	ldmia r0!, {r1-r4}
	ldmdb r0!, {r1-r4}

	mov r0, 0xFF00FF00
	mov r0, 0x1234
	movt r0, 0x5678

	mov r1, 17
	mov r2, 5




	pop {r0-r5, lr}
	bx lr


data1:
.word _vma_start_value

.end
