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
	VT = 0, MCU2 = 1
} UART_Target;

extern void UART_Init();

extern void UART_AsyncTransmitString(UART_Target target, const char* string);
extern char* UART_BlockReceiveString(UART_Target target);



#endif /* CORE_INC_UART_DRIVER_H_ */
