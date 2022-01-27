/*
 * uart_driver.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "uart_driver.h"
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "usart.h"

TaskHandle_t UART_TaskHandle;
QueueHandle_t UART_QueueHandle;
SemaphoreHandle_t UART_MutexHandle;

static void UART_TransmitTask(void *p)
{
	uint8_t buffer;

	while(1)
	{
		xQueueReceive(UART_QueueHandle, &buffer, portMAX_DELAY);

		HAL_UART_Transmit_IT(&huart1, &buffer, sizeof(uint8_t));

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance == huart1.Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_TaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}

}


void UART_Init()
{

	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, NULL, 4, &UART_TaskHandle);
	UART_QueueHandle = xQueueCreate(64, sizeof(uint8_t));
	UART_MutexHandle = xSemaphoreCreateMutex();

}


void UART_AsyncTransmitString(const char *s)
{
	if(s != NULL)
	{
		xSemaphoreTake(UART_MutexHandle, portMAX_DELAY);

		for(int i = 0; i < strlen(s); i++)
		{
			xQueueSendToBack(UART_QueueHandle, s + i, portMAX_DELAY);
		}

		xSemaphoreGive(UART_MutexHandle);
	}
}

