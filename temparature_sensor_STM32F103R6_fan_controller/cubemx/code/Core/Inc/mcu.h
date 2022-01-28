/*
 * mcu.h
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#ifndef CORE_INC_MCU_H_
#define CORE_INC_MCU_H_

extern void MCU_Init();

typedef enum
{
	TURNED_OFF, SLOW, FAST
} FanState;

/*
 * Tasks -> priority
 * MCU_Task -> 5
 * UART_TransmitTask -> 4
 * LCD_Task -> 2
 * TEMP_Task -> 2
 */

#endif /* CORE_INC_MCU_H_ */
