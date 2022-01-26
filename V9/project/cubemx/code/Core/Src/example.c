/*
 * example.c
 *
 *  Created on: Jan 26, 2022
 *      Author: matij
 */

#include "example.h"

#include "FreeRTOS.h"
#include "task.h"
#include "uart_driver.h"

#include "gpio.h"

static void exampleTask(void* p)
{
	UART_AsyncTransmitCharacter('M');
	UART_AsyncTransmitString("IPS_");
	UART_AsyncTransmitDecimal(2020);
	UART_AsyncTransmitCharacter('\r');

	while(1)
	{
//		char character = UART_BlockReceiveCharacter();
//		UART_AsyncTransmitCharacter(character);
//
//		char* string = UART_BlockReceiveString();
//		UART_AsyncTransmitString(string);
//		vPortFree(string);

		UART_AsyncTransmitString("Unesi diodu [1-4]\r");
		uint32_t index = UART_BlockReceiveDecimal();
		if(index >= 1 && index <= 4)
		{
			HAL_GPIO_TogglePin(GPIOC, 0x01 << (index - 1));
		}
		else
		{
			UART_AsyncTransmitString("Redni broj izvan opsega\r\r");
		}
//		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void exampleInit()
{
	xTaskCreate(exampleTask, "exampleTask", 128, NULL, 10, NULL);
	UART_Init();
}

