/*
 * homework.h
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#ifndef CORE_INC_HOMEWORK_H_
#define CORE_INC_HOMEWORK_H_

typedef enum
{
	TURNED_OFF, SLOW, FAST
} FanState;

extern void homeworkInit();

/*
 * task -> priority
 * LCD_Task -> 2
 * UART_Transmit -> 4
 * UART_Receive -> 20
 * TEMP_Task -> 2
 * homeworkTask -> 5
 * KeyPad -> 26
 */


#endif /* CORE_INC_HOMEWORK_H_ */
