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

typedef struct
{
	uint8_t motor;
	uint8_t velocity;
} MotorCommand;


extern void UART_AsyncTransmitString(UART_Target, const char*);
extern void UART_AsyncTransmitCharacter(UART_Target, char);
extern void UART_AsyncTransmitDecimal(UART_Target, uint32_t);

extern void UART_AsyncTransmitxMotorCommand(UART_Target, MotorCommand);

extern char* UART_BlockReceiveString(UART_Target);
extern char UART_BlockReceiveCharacter(UART_Target);
extern uint32_t UART_BlockReceiveDecimal(UART_Target);

#endif /* CORE_INC_UART_DRIVER_H_ */
