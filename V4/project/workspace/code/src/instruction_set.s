.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb

.extern _vma_data_start
.global instruction_set

.section .text.instruction_set
.type instruction_set, %function
instruction_set:
	push {r0-r5, lr}

#	str lr, [sp, -4]!

	ldr r0, data1

	ldr r0, =data1
	ldr r0, [r0]

	adr r0, data1
	ldr r1, [r0]

	ldrb r1, [r0, 3]
	ldrb r1, [r0], 1
	ldrh r1, [r0, 1]!

	ldm r0, {r1-r4}
	ldmia r0!, {r1, r2, r3, r4}
	ldmdb r0!, {r1, r2, r3, r4}

	mov r0, 0xFF00FF00
	#mov r0, 0x12345678
	mov r0, 0x5678
	movt r0, 0x1234

	movt r0, 0x1234
	mov r0, 0x5678

	mov r1, 17
	mov r2, 5

	sub r1, r2
	sub r0, r1, r2
	sub r0, r1, r2, LSL 2
	sub r0, r1, 9

	mov r3, 0xF0000000
	mov r4, 2

	lsl r0, r3, 4
	lsls r0, r3, 4
	lsl r0, r3, r4
	lsls r0, r3, r4

	cmp r1, 13

	ittet ne
	movne r0,5
	movne r1,4
	moveq r2,3
	movne r3,2

	#itttt ne
	movne r0,1
	movsne r1,0
	movne r2,-1
	movsne r3,-2


#	ldr lr, [sp], 4

	pop {r0-r5, lr}

	bx lr

data1:
.word _vma_data_start

	.end
