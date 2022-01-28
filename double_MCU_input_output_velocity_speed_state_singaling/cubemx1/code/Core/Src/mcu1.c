/*
 * mcu.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "mcu1.h"

#include "uart_driver.h"

#include "FreeRTOS.h"
#include "task.h"

void MCU1_Task(void* p)
{

	UART_AsyncTransmitString(VT, "MIPS");
	UART_AsyncTransmitCharacter(VT, '_');
	UART_AsyncTransmitDecimal(VT, 2021);
	UART_AsyncTransmitCharacter(VT, '\r');

	char* s = UART_BlockReceiveString(VT);
	UART_AsyncTransmitString(VT, s);
	vPortFree(s);
	UART_AsyncTransmitCharacter(VT, '\r');

	char c = UART_BlockReceiveCharacter(VT);
	UART_AsyncTransmitCharacter(VT, c);
	UART_AsyncTransmitCharacter(VT, '\r');

//	uint32_t d = UART_BlockReceiveDecimal(VT);
//	UART_AsyncTransmitDecimal(VT, d);
	UART_AsyncTransmitCharacter(VT, '\r');

	while(1)
	{
		UART_AsyncTransmitString(VT, "MIPS");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}


void MCU1_Init()
{
	UART_Init();

	xTaskCreate(MCU1_Task, "mcu", 64, NULL, 5, NULL);

}
