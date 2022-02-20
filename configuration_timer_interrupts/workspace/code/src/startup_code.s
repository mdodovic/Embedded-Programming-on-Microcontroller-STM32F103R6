.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb


.weak nmi_handler
.thumb_set nmi_handler, default_handler

.weak hard_fault_handler
.thumb_set hard_fault_handler, default_handler

.weak memory_manage_handler
.thumb_set memory_manage_handler, default_handler

.weak bus_fault_handler
.thumb_set bus_fault_handler, default_handler

.weak usage_fault_handler
.thumb_set usage_fault_handler, default_handler

.weak svcall_handler
.thumb_set svcall_handler, default_handler

.weak pendsv_handler
.thumb_set pendsv_handler, default_handler

.weak systick_handler
.thumb_set systick_handler, default_handler

.weak irq0_wwdg_handler
.thumb_set irq0_wwdg_handler, default_handler

.weak irq1_pvd_handler
.thumb_set irq1_pvd_handler, default_handler

.weak irq2_tamper_handler
.thumb_set irq2_tamper_handler, default_handler

.weak irq3_rtc_handler
.thumb_set irq3_rtc_handler, default_handler

.extern _main_stack_initial_value

.section .vector_table, "a"
.word _main_stack_initial_value
.word reset_handler
.word nmi_handler
.word hard_fault_handler
.word memory_manage_handler
.word bus_fault_handler
.word usage_fault_handler
.rept 4
	.word default_handler
.endr
.word svcall_handler
.rept 2
	.word default_handler
.endr
.word pendsv_handler
.word systick_handler
.word irq0_wwdg_handler
.word irq1_pvd_handler
.word irq2_tamper_handler
.word irq3_rtc_handler


.extern main
.extern _vma_start_address
.extern _vma_end_address
.extern _lma_start_address

.section .text.reset
.type reset_handler, %function
reset_handler:
	ldr r0, =_vma_start_address
	ldr r1, =_vma_end_address
	ldr r2, =_lma_start_address

	cmp r0, r1 // if r0 < r1 move data
	beq no_more_data

more_to_move:
	ldr r3, [r2], 4
	str r3, [r0], 4
	cmp r0, r1 // if r0 < r1 move data
	blo more_to_move

no_more_data:
	b main
infinite_loop_reset_handler:
	b infinite_loop_reset_handler


.section .text.default
.type default_handler, %function
default_handler:
infinite_loop_default_handler:
	b infinite_loop_default_handler


