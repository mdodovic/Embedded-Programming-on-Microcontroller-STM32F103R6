.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb

.extern _main_stack_pointer

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


.section .vector_table, "a"
.word _main_stack_pointer
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
.rept 64
	.word default_handler
.endr


.extern main
.extern _vma_data_start
.extern _vma_data_end
.extern _lma_data_start

.section .text.reset_handler
.type reset_handler, %function
reset_handler:
	ldr r0, =_vma_data_start
	ldr r1, =_vma_data_end
	ldr r2, =_lma_data_start
	cmp r0, r1 // if r0 < r1: move data from lma to vma
	beq end_of_moving
move_data:
	ldr r3, [r2], 4
	str r3, [r0], 4
	cmp r0, r1 // if r0 < r1: continue with moving
	blo move_data

end_of_moving:
	b main
infinite_reset_handler:
	b infinite_reset_handler


.section .text.default_handler
.type default_handler, %function
default_handler:
infinite_default_handler:
	b infinite_default_handler

.end


