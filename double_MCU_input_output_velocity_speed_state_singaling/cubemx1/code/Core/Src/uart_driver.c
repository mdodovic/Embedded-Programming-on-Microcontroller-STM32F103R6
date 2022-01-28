/*
 * uart_driver.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "uart_driver.h"

#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "usart.h"

QueueHandle_t UART_TransmitQueueHandle[2];
SemaphoreHandle_t UART_TransmitMutexHandle[2];
TaskHandle_t UART_TransmitTaskHandle[2];

// Transmit

static void UART_TransmitTask(void* p)
{
	UART_Target target = (UART_Target) p;

	uint8_t buffer;

	while(1)
	{
		xQueueReceive(UART_TransmitQueueHandle[target], &buffer, portMAX_DELAY);

		HAL_UART_Transmit_IT(&huart1, &buffer, sizeof(uint8_t));

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t woken = pdFALSE;
	if(huart->Instance == huart1.Instance)
	{
		vTaskNotifyGiveFromISR(UART_TransmitTaskHandle[VT], &woken);
	}
	else {
		vTaskNotifyGiveFromISR(UART_TransmitTaskHandle[MCU2], &woken);
	}
	portYIELD_FROM_ISR(woken);
}

// Receive

QueueHandle_t UART_ReceiveQueueHandle[2];
SemaphoreHandle_t UART_ReceiveMutexHandle[2];
TaskHandle_t UART_ReceiveTaskHandle[2];


void UART_Init()
{
	// VT - Transmit
	UART_TransmitQueueHandle[VT] = xQueueCreate(64, sizeof(uint32_t));
	UART_TransmitMutexHandle[VT] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void *) VT, 4, &UART_TransmitTaskHandle[VT]);

	// MCU2 - Receive
	// VT - Transmit
	// MCU2 - Receive
}


void UART_AsyncTransmitString(UART_Target target, const char* s)
{
	if(s != NULL)
	{
		xSemaphoreTake(UART_TransmitMutexHandle[target], portMAX_DELAY);

		for(uint32_t i = 0; i < strlen(s); i++)
		{
			xQueueSendToBack(UART_TransmitQueueHandle[target], s + i, portMAX_DELAY);
		}

		xSemaphoreGive(UART_TransmitMutexHandle[target]);
	}
}

void UART_AsyncTransmitCharacter(UART_Target target, char c)
{
	xSemaphoreTake(UART_TransmitMutexHandle[target], portMAX_DELAY);

	xQueueSendToBack(UART_TransmitQueueHandle[target], &c, portMAX_DELAY);

	xSemaphoreGive(UART_TransmitMutexHandle[target]);

}
void UART_AsyncTransmitDecimal(UART_Target target, uint32_t d)
{
	xSemaphoreTake(UART_TransmitMutexHandle[target], portMAX_DELAY);

	char s[32];
	int i = 32;
	while(i >= 0 && d > 0)
	{
		s[--i] = d % 10 + '0';
		d /= 10;
	}

	for(uint32_t j = i; j < 32; j++)
	{
		xQueueSendToBack(UART_TransmitQueueHandle[target], s + j, portMAX_DELAY);
	}

	xSemaphoreGive(UART_TransmitMutexHandle[target]);

}



