/*
 * uart_driver.h
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#ifndef CORE_INC_UART_DRIVER_H_
#define CORE_INC_UART_DRIVER_H_


extern void UART_Init();

extern void UART_AsyncTransmitString(const char *s);

#endif /* CORE_INC_UART_DRIVER_H_ */
