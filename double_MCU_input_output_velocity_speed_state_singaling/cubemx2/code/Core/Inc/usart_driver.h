/*
 * usart_driver.h
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#ifndef CORE_INC_USART_DRIVER_H_
#define CORE_INC_USART_DRIVER_H_


#include <stdint.h>

void UART_Init();

typedef struct
{
	uint8_t motor;
	uint8_t velocity;
} MotorCommand;


extern MotorCommand UART_BlockReceiveMotorCommand();


#endif /* CORE_INC_USART_DRIVER_H_ */
