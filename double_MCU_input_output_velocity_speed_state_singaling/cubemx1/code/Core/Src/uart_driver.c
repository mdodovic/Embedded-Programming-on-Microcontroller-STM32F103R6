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

UART_HandleTypeDef *phuart[2] = { &huart1, &huart2 };

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

		HAL_UART_Transmit_IT(phuart[target], &buffer, sizeof(uint8_t));

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t woken = pdFALSE;
	if(huart->Instance == phuart[VT]->Instance)
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

void UART_ReceiveTask(void* p)
{
	UART_Target target = (UART_Target) p;

	uint8_t buffer;

	while(1)
	{
		HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		xQueueSendToBack(UART_ReceiveQueueHandle[target], &buffer, portMAX_DELAY);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t woken = pdFALSE;
	if(huart->Instance == phuart[VT]->Instance)
	{
		vTaskNotifyGiveFromISR(UART_ReceiveTaskHandle[VT], &woken);
	}
	else {
		vTaskNotifyGiveFromISR(UART_ReceiveTaskHandle[MCU2], &woken);
	}
	portYIELD_FROM_ISR(woken);
}


void UART_Init()
{
	// VT - Transmit
	UART_TransmitQueueHandle[VT] = xQueueCreate(64, sizeof(uint8_t));
	UART_TransmitMutexHandle[VT] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void *) VT, 4, &UART_TransmitTaskHandle[VT]);

	// VT - Receive
	UART_ReceiveQueueHandle[VT] = xQueueCreate(64, sizeof(uint8_t));
	UART_ReceiveMutexHandle[VT] = xSemaphoreCreateMutex();
	xTaskCreate(UART_ReceiveTask, "UART_ReceiveTask", 64, (void *) VT, 20, &UART_ReceiveTaskHandle[VT]);

	// MCU2 - Transmit
	UART_TransmitQueueHandle[MCU2] = xQueueCreate(64, sizeof(uint8_t));
	UART_TransmitMutexHandle[MCU2] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void *) MCU2, 4, &UART_TransmitTaskHandle[MCU2]);


}

void UART_AsyncTransmitxMotorCommand(UART_Target target, MotorCommand motorCommand)
{
	xSemaphoreTake(UART_TransmitMutexHandle[target], portMAX_DELAY);

	xQueueSendToBack(UART_TransmitQueueHandle[target], &motorCommand.motor,	portMAX_DELAY);
	xQueueSendToBack(UART_TransmitQueueHandle[target], &motorCommand.velocity, portMAX_DELAY);

	xSemaphoreGive(UART_TransmitMutexHandle[target]);
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

char* UART_BlockReceiveString(UART_Target target)
{

	xSemaphoreTake(UART_ReceiveMutexHandle[target], portMAX_DELAY);

	char* s = pvPortMalloc(32);

	if(s != NULL)
	{

		char c = '\0';
		uint32_t i = 0;
		while(c != '\r' && i < 32)
		{
			xQueueReceive(UART_ReceiveQueueHandle[target], &c, portMAX_DELAY);
			s[i++] = c;
		}
		s[--i] = '\0';
	}

	xSemaphoreGive(UART_ReceiveMutexHandle[target]);

	return s;
}

char UART_BlockReceiveCharacter(UART_Target target)
{
	xSemaphoreTake(UART_ReceiveMutexHandle[target], portMAX_DELAY);

	char c;
	xQueueReceive(UART_ReceiveQueueHandle[target], &c, portMAX_DELAY);

	xSemaphoreGive(UART_ReceiveMutexHandle[target]);
	return c;
}

uint32_t UART_BlockReceiveDecimal(UART_Target target)
{
	xSemaphoreTake(UART_ReceiveMutexHandle[target], portMAX_DELAY);

	uint32_t d = 0;

	char c = '\0';
	while(c != '\r')
	{
		xQueueReceive(UART_ReceiveQueueHandle[target], &c, portMAX_DELAY);
		if (c >= '0' && c <= '9')
		{
			d = d * 10 + (c - '0');
		}
	}

	xSemaphoreGive(UART_ReceiveMutexHandle[target]);
	return d;
}



