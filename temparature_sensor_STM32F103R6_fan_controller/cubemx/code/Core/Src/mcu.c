/*
 * mcu.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "mcu.h"

#include "FreeRTOS.h"
#include "task.h"

#include "uart_driver.h"

void MCU_Task(void* p)
{
	UART_AsyncTransmitString("MIPS\r");

	while(1)
	{

	}
}



void MCU_Init()
{
	UART_Init();

	xTaskCreate(MCU_Task, "MCU_Task", 128, NULL, 5, NULL);

}
