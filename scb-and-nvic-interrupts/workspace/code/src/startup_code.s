.cpu cortex-m3
.fpu softvfp
.syntax unified
.thumb

.extern _main_stack_pointer_value

.weak nmi_handler
.thumb_set nmi_handler, default_handler

.weak hard_fault_handler
.thumb_set hard_fault_handler, default_handler

.weak memory_fault_handler
.thumb_set memory_fault_handler, default_handler

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

.weak irq0_WWDG_handler
.thumb_set irq0_WWDG_handler, default_handler

.weak irq1_PVD_handler
.thumb_set irq1_PVD_handler, default_handler

.weak irq2_TAMPER_handler
.thumb_set irq2_TAMPER_handler, default_handler

.weak irq3_RTC_handler
.thumb_set irq3_RTC_handler, default_handler


.section .vector_table, "a"
.word _main_stack_pointer_value  // 0
.word reset_handler              // 1
.word nmi_handler                // 2
.word hard_fault_handler         // 3
.word memory_fault_handler       // 4
.word bus_fault_handler          // 5
.word usage_fault_handler        // 6
.rept 4						  // reserver
	.word default_handler     // 7 - 10
.endr
.word svcall_handler             // 11
.rept 2						  // reserver
	.word default_handler     // 12 - 13
.endr
.word pendsv_handler             // 14
.word systick_handler            // 15
.word irq0_WWDG_handler          // 16
.word irq1_PVD_handler           // 17
.word irq2_TAMPER_handler        // 18
.word irq3_RTC_handler           // 19
.rept 64                      // irq
	.word default_handler     // 20 - 83
.endr

.extern _vma_data_start
.extern _vma_data_end
.extern _lma_data_start
.extern main

.section .text.reset_handler
.type reset_handler, %function
reset_handler:
	ldr r0, =_lma_data_start
	ldr r1, =_vma_data_start
	ldr r2, =_vma_data_end

	cmp r1, r2 // r1 < r2
	beq jump_to_main // if r1 == r2 jmp
copy_loop:
	ldr r3,[r0],#4
	str r3,[r1],#4
	cmp r1, r2 // r1 < r2
	blo copy_loop // if r1 < r2 jmp

jump_to_main:
	b main
infinite_loop_reset:
	b infinite_loop_reset

.section .text.default_handler
.type default_handler, %function
default_handler:
infinite_loop_default:
	b infinite_loop_default

.end

