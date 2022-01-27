/*
 * mcu1.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "mcu1.h"


#include "FreeRTOS.h"

#include "task.h"

#include "queue.h"
#include "uart_driver.h"


static void mcu1Task(void *p)
{

	while(1)
	{
		UART_AsyncTransmitString(VT, "MPIS\r");
		char* command =	UART_BlockReceiveString(VT);
		UART_AsyncTransmitString(VT, command);

		//vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void MCU1_Init()
{
	UART_Init();

	xTaskCreate(mcu1Task, "mcu1Task", 64, NULL, 10, NULL);

}
