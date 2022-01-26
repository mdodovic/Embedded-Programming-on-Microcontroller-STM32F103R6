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

static void exampleTask(void* p)
{
	while(1)
	{
		UART_AsyncTransmitCharacter('a');
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void exampleInit()
{
	xTaskCreate(exampleTask, "exampleTask", 128, NULL, 10, NULL);
	UART_Init();
}

