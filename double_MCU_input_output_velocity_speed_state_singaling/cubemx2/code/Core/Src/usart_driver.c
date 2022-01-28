/*
 * usart_driver.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "usart_driver.h"

#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "usart.h"

// Receive

static TaskHandle_t UART_ReceiveTaskHandle;
static QueueHandle_t UART_ReceiveQueueHandle;
static SemaphoreHandle_t UART_ReceiveMutexHandle;

static void UART_ReceiveTask(void *parameters)
{
	uint8_t buffer;
	while (1)
	{
		HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		xQueueSendToBack(UART_ReceiveQueueHandle, &buffer, portMAX_DELAY);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart1.Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_ReceiveTaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}


void UART_Init()
{

	// MCU1 - Receive
	xTaskCreate(UART_ReceiveTask, "receiveTask", 128, NULL, 20, &UART_ReceiveTaskHandle);
	UART_ReceiveQueueHandle = xQueueCreate(128, sizeof(uint8_t));
	UART_ReceiveMutexHandle = xSemaphoreCreateMutex();
}



MotorCommand UART_BlockReceiveMotorCommand()
{
	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);

	MotorCommand motorCommand;

	xQueueReceive(UART_ReceiveQueueHandle, &motorCommand.motor, portMAX_DELAY);
	xQueueReceive(UART_ReceiveQueueHandle, &motorCommand.velocity, portMAX_DELAY);

	xSemaphoreGive(UART_ReceiveMutexHandle);

	return motorCommand;
}

