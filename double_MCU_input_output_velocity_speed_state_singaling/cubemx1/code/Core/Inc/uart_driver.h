/*
 * uart_driver.h
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#ifndef CORE_INC_UART_DRIVER_H_
#define CORE_INC_UART_DRIVER_H_

#include <stdint.h>

void UART_Init();

typedef enum
{
	VT = 0, MCU2 = 1
} UART_Target;


extern void UART_AsyncTransmitString(UART_Target, const char*);
extern void UART_AsyncTransmitCharacter(UART_Target, char);
extern void UART_AsyncTransmitDecimal(UART_Target, uint32_t);

//	UART_AsyncTransmitCharacter(VT, '\r');
//
//	char* s = UART_BlockReceiveString(VT);
//	UART_AsyncTransmitString(VT, "MIPS");
//	UART_AsyncTransmitCharacter(VT, '\r');
//
//	char c = UART_BlockReceiveCharacter(VT);
//	UART_AsyncTransmitCharacter(VT, c);
//	UART_AsyncTransmitCharacter(VT, '\r');
//
//	uint32_t d = UART_BlockReceiveDecimal(VT);
//	UART_AsyncTransmitDecimal(VT, d);
//	UART_AsyncTransmitCharacter(VT, '\r');


#endif /* CORE_INC_UART_DRIVER_H_ */
