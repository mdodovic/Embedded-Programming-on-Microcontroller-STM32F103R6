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

static QueueHandle_t UART_ReceiveQueueHandle;
static TaskHandle_t UART_ReceiveTaskHandle;
static SemaphoreHandle_t UART_ReceiveMutexHandle;


// Transmit

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
		portYIELD_FROM_ISR(woken);
	}
}

// Receive

static void UART_ReceiveTask(void* p)
{
	uint8_t buffer;

	while(1)
	{

		HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		xQueueSendToBack(UART_ReceiveQueueHandle, &buffer, portMAX_DELAY);

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_ReceiveTaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}


// General

void UART_Init()
{

	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 128, NULL, 4, &UART_TransmitTaskHandle);
	UART_TransmitQueueHandle = xQueueCreate(128, sizeof(uint8_t));
	UART_TransmitMutexHandle = xSemaphoreCreateMutex();

	xTaskCreate(UART_ReceiveTask, "UART_ReceiveTask", 128, NULL, 20, &UART_ReceiveTaskHandle);
	UART_ReceiveQueueHandle = xQueueCreate(128, sizeof(uint8_t));
	UART_ReceiveMutexHandle = xSemaphoreCreateMutex();


}

// transmit

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

void UART_AsyncTransmitDecimal(uint32_t decimal)
{

	xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);

	char digits[32];
	uint32_t index = 32;
	while(index >= 0 && decimal != 0)
	{
		digits[--index] = decimal % 10 + '0';
		decimal /= 10;
	}

	for(uint32_t i = index; i < 32; i++)
	{
		xQueueSendToBack(UART_TransmitQueueHandle, digits + i, portMAX_DELAY);
	}

	xSemaphoreGive(UART_TransmitMutexHandle);
}

// receive util

char UART_BlockReceiveCharacter()
{
	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);

	char c;
	xQueueReceive(UART_ReceiveQueueHandle, &c, portMAX_DELAY);

	xSemaphoreGive(UART_ReceiveMutexHandle);

	return c;
}

char* UART_BlockReceiveString()
{
	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);

	char *string = pvPortMalloc(64);
	uint32_t i = 0;
	if(string != NULL)
	{
		char c = '\0';
		while(c != '\r' && i < 64)
		{
			xQueueReceive(UART_ReceiveQueueHandle, &c, portMAX_DELAY);
			string[i++] = c;
		}

		string[--i] = '\0';
	}

	xSemaphoreGive(UART_ReceiveMutexHandle);

	return string;
}

uint32_t UART_BlockReceiveDecimal()
{

	xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);

	uint32_t decimal = 0;

	char c = '\0';
	while(c != '\r')
	{
		xQueueReceive(UART_ReceiveQueueHandle, &c, portMAX_DELAY);
		if(c >= '0' && c <= '9')
		{
			decimal = decimal * 10 + c - '0';
		}
	}

	xSemaphoreGive(UART_TransmitMutexHandle);

	return decimal;
}



