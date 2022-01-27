/*
 * uart_driver.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "uart_driver.h"

#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "usart.h"

SemaphoreHandle_t UART_MutexHandle;
QueueHandle_t UART_QueueHandle;
TaskHandle_t UART_ReceiveTaskHandle;


void UART_ReceiveTask(void* p)
{
	uint8_t buffer;
	while(1)
	{
		HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		xQueueSendToBack(UART_QueueHandle, &buffer, portMAX_DELAY);
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

void UART_Init()
{

	UART_MutexHandle = xSemaphoreCreateMutex();
	UART_QueueHandle = xQueueCreate(128, sizeof(uint8_t));
	xTaskCreate(UART_ReceiveTask, "UART_ReceiveTask", 64, NULL, 20, &UART_ReceiveTaskHandle);

}



// Util

MotorCommand UART_BlockReceiveMotorCommand()
{
	MotorCommand mc = {0, 0};

	xSemaphoreTake(UART_MutexHandle, portMAX_DELAY);

	xQueueReceive(UART_QueueHandle, &mc.motor, portMAX_DELAY);
	xQueueReceive(UART_QueueHandle, &mc.velocity, portMAX_DELAY);

	xSemaphoreGive(UART_MutexHandle);

	return mc;
}
