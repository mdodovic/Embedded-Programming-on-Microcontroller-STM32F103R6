	.cpu cortex-m3
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.global	nepromenjiva
	.section	.rodata
	.align	2
	.type	nepromenjiva, %object
	.size	nepromenjiva, 4
nepromenjiva:
	.word	6
	.global	promenjiva
	.data
	.align	2
	.type	promenjiva, %object
	.size	promenjiva, 4
promenjiva:
	.word	7
	.comm	neinicijalizovana,4,4
	.text
	.align	1
	.global	main
	.arch armv7-m
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	add	r7, sp, #0
	movs	r3, #6
	adds	r2, r3, #5
	ldr	r3, .L3
	ldr	r3, [r3]
	add	r3, r3, r2
	ldr	r2, .L3+4
	str	r3, [r2]
	movs	r3, #0
	mov	r0, r3
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L4:
	.align	2
.L3:
	.word	promenjiva
	.word	neinicijalizovana
	.size	main, .-main
	.ident	"GCC: (xPack GNU Arm Embedded GCC, 64-bit) 10.2.1 20201103 (release)"
