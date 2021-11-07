.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb

.extern _vma_data_start

.global instruction_set

.section .text.instruction_set
.type instruction_set, %function
instruction_set:
	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.4.7 PUSH and POP
	push {r0-r7, lr}				@ stm sp!, {r0-r7, lr}

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.4.5 LDR, PC-relative
	ldr r0, data                    @ r0 <= mem32[data]

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.4.2 LDR and STR, immediate offset
	ldr r1, =data                   @ r1 <= data
	ldr r2, [r1]                    @ r2 <= mem32[r1]

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.4.1 ADR
	adr r3, data                    @ r3 <= data
	ldr r4, [r3]                    @ r4 <= mem32[r3]
	ldrb r4, [r3, 3]                @ r4 <= zero-extend24(mem8[r3 + 3])
	ldrb r4, [r3], 1                @ r4 <= zero-extend24(mem8[r3]); r3 <= r3 + 1
	ldrh r4, [r3, 1]!               @ r4 <= zero-extend16(mem16[r3 + 1]); r3 <= r3 + 1

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.4.6 LDM and STM
	ldm r0, {r1-r4}                 @ r1 <= mem32[r0]; r2 <= mem32[r0 + 4]; r3 <= mem32[r0 + 8]; r4 <= mem32[r0 + 12];
	ldm r0, {r1, r2, r3, r4}        @ r1 <= mem32[r0]; r2 <= mem32[r0 + 4]; r3 <= mem32[r0 + 8]; r4 <= mem32[r0 + 12];
	ldmia r0, {r1, r2, r3, r4}      @ r1 <= mem32[r0]; r2 <= mem32[r0 + 4]; r3 <= mem32[r0 + 8]; r4 <= mem32[r0 + 12];
	ldmia r0!, {r1-r4}              @ r1 <= mem32[r0]; r2 <= mem32[r0 + 4]; r3 <= mem32[r0 + 8]; r4 <= mem32[r0 + 12]; r0 <= r0 + 16
	ldmdb r0!, {r1-r4}              @ r4 <= mem32[r0 - 4]; r3 <= mem32[r0 - 8]; r2 <= mem32[r0 - 12]; r4 <= mem32[r0 - 16]; r0 <= r0 - 16

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.5.6 MOV and MVN
	mov r0, 0xFF00FF00              @ r0 <= 0xFF00FF00
	# mov r0, 0x12345678

	mov r0, 0x5678                  @ r0 <= zero-extend16(0x5678)
	movt r0, 0x1234                 @ r0[31:16] <= 0x1234

	mov r1, 17                      @ r1 <= 17
	mov r2, 5                       @ r2 <= 5

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.3.3 Flexible second operand
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.5.1 ADD, ADC, SUB, SBC, and RSB
	sub r1, r2                      @ r1 <= r1 -r2

	sub r0, r1, r2                  @ r0 <= r1 - r2
	sub r0, r1, 9                   @ r0 <= r1 - 9
	sub r0, r1, r2, LSL 2           @ r0 <= r1 - (r2 << 2)

	mov r3, 0xF0000000              @ r0 <= 0xF0000000
	mov r4, 2

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.3.4 Shift operations
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.5.3 ASR, LSL, LSR, ROR, and RRX
	lsl r0, r3, 4                   @ r0 <= r3 << 4
	lsls r0, r3, 4                  @ r0 <= r3 << 4; update flags (N, Z, C)
	lsl r0, r3, r4                  @ r0 <= r3 << r4
	lsls r0, r3, r4                 @ r0 <= r3 << r4; update flags (N, Z, C)

	subs r0, r1, 12                 @ r0 <= r1 - 12; update flags (N, Z, C, V)
	subs r0, r1, 13                 @ r0 <= r1 - 12; update flags (N, Z, C, V)

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.3.7 Conditional execution
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.8.7 IT
	itttt ne
	movne r0, 5
	movsne r1, 4
	movne r2, 3
	movsne r3, 2

	#itttt ne                       @ FLAGS_AS += -Wa,-mimplicit-it=thumb
	movne r0, 1
	movsne r1, 0
	movne r2, -1
	movsne r3, -2

	pop {r0-r7, lr}

	# izvor informacija:
	# 01_STM32F103_Cortex_M3_Programming_Manual -> 3.8.5 B, BL, BX, and BLX
	bx lr
data:
.word _vma_data_start

	.end
