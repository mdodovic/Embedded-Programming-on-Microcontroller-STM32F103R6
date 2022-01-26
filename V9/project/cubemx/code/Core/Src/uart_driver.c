/*
 * uart_driver.c
 *
 *  Created on: Jan 26, 2022
 *      Author: matij
 */

#include "uart_driver.h"

#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "usart.h"


#include "string.h"

static QueueHandle_t UART_TransmitQueueHandle;

static TaskHandle_t UART_TransmitTaskHandle;

static SemaphoreHandle_t UART_TransmitMutexHandle;

static void UART_TransmitTask(void* p)
{
	uint8_t buffer;

	while(1)
	{

		xQueueReceive(UART_TransmitQueueHandle, &buffer, portMAX_DELAY);

		HAL_UART_Transmit_IT(&huart1, &buffer, sizeof(uint8_t));

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_TransmitTaskHandle, &woken);

	}
}

void UART_Init()
{

	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 128, NULL, 4, &UART_TransmitTaskHandle);

	UART_TransmitQueueHandle = xQueueCreate(128, sizeof(uint8_t));

	UART_TransmitMutexHandle = xSemaphoreCreateMutex();
}


void UART_AsyncTransmitCharacter(char character)
{
	xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);

	xQueueSendToBack(UART_TransmitQueueHandle, &character, portMAX_DELAY);

	xSemaphoreGive(UART_TransmitMutexHandle);
}

void UART_AsyncTransmitString(char const* string)
{
	if(string != NULL)
	{
		xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);

		for(uint32_t i = 0; i < strlen(string); i++)
		{
			xQueueSendToBack(UART_TransmitQueueHandle, string + i, portMAX_DELAY);
		}

		xSemaphoreGive(UART_TransmitMutexHandle);
	}
}

