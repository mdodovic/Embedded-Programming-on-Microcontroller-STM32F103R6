/*
 * uart_driver.h
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#ifndef CORE_INC_UART_DRIVER_H_
#define CORE_INC_UART_DRIVER_H_

typedef enum
{
	TERMINAL = 0,
	MCU2 = 1
} UART_Target;

extern void UART_Init();

#endif /* CORE_INC_UART_DRIVER_H_ */
