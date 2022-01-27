/*
 * uart_driver.h
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#ifndef CORE_INC_UART_DRIVER_H_
#define CORE_INC_UART_DRIVER_H_

#include <stdint.h>

typedef struct
{
	uint8_t motor;
	uint8_t velocity;
} MotorCommand;



extern void UART_Init();

extern MotorCommand UART_BlockReceiveMotorCommand();


#endif /* CORE_INC_UART_DRIVER_H_ */
