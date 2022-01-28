/*
 * mcu1.h
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#ifndef CORE_INC_MCU1_H_
#define CORE_INC_MCU1_H_

#include <stdint.h>

extern void MCU1_Init();
extern uint8_t velocity[2];

/*
 * task -> priority
 * MCU_Task -> 5
 * UART_TransmitTask[VT] -> 4
 * UART_TransmitTask[MCU2] -> 4
 * UART_ReceiveTask[VT] -> 20
 * UART_ReceiveTask[MCU2] -> 20
 */

#endif /* CORE_INC_MCU1_H_ */
