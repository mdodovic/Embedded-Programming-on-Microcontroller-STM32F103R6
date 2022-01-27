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


static UART_HandleTypeDef *phuart[2] = {&huart1, &huart2};

static QueueHandle_t UART_QueueTransmitHandle[2];
static SemaphoreHandle_t UART_MutexTransmitHandle[2];
static TaskHandle_t UART_TaskTransmitHandle[2];

static QueueHandle_t UART_QueueReceiveHandle[2];
static SemaphoreHandle_t UART_MutexReceiveHandle[2];
static TaskHandle_t UART_TaskReceiveHandle[2];

/* Transmit */

// task for transmitting

static void UART_TransmitTask(void* p)
{
	UART_Target target = (UART_Target) p;
	uint8_t buffer;

	while(1)
	{

		xQueueReceive(UART_QueueTransmitHandle[target], &buffer, portMAX_DELAY); // use character from queue
		HAL_UART_Transmit_IT(phuart[target], &buffer, sizeof(uint8_t));			 // transmit it to the pin TX

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until it is transmitted

	}
}

// callback after the transmission is compledet

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == phuart[TERMINAL])
	{
		BaseType_t woken;
		vTaskNotifyGiveFromISR(UART_TaskTransmitHandle[TERMINAL], &woken);
		portYIELD_FROM_ISR(woken);
	}
}



void UART_Init()
{
	/* Transmit from Virtual Terminal */

	UART_QueueTransmitHandle[TERMINAL] = xQueueCreate(128, sizeof(uint8_t));
	UART_MutexTransmitHandle[TERMINAL] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void *) TERMINAL, 4, UART_TaskTransmitHandle[TERMINAL]);

}
