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

.section .text.reset_handler
.type reset_handler, %function
reset_handler:

.section .text.default_handler
.type default_handler, %function
default_handler:
	b default_handler
.end
