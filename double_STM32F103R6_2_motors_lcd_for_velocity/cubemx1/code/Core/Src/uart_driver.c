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

#include <string.h>

static UART_HandleTypeDef *phuart[2] = {&huart1, &huart2};

static QueueHandle_t UART_QueueTransmitHandle[2];
static SemaphoreHandle_t UART_MutexTransmitHandle[2];
static TaskHandle_t UART_TaskTransmitHandle[2];

//static QueueHandle_t UART_QueueReceiveHandle[2];
//static SemaphoreHandle_t UART_MutexReceiveHandle[2];
//static TaskHandle_t UART_TaskReceiveHandle[2];

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

// callback after the transmission is completed

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// transmission finished: next transmission can start
	if(huart->Instance == phuart[VT]->Instance)
	{
		BaseType_t woken;
		vTaskNotifyGiveFromISR(UART_TaskTransmitHandle[VT], &woken);
		portYIELD_FROM_ISR(woken);
	}
}



void UART_Init()
{
	/* Transmit from Virtual Terminal */

	UART_QueueTransmitHandle[VT] = xQueueCreate(128, sizeof(uint8_t));
	UART_MutexTransmitHandle[VT] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void *) VT, 4, &UART_TaskTransmitHandle[VT]);

}


/* utility: Transmit */

void UART_AsyncTransmitString(UART_Target target, const char* string)
{
	if(string != NULL)
	{
		xSemaphoreTake(UART_MutexTransmitHandle[target], portMAX_DELAY);

		for(uint32_t i = 0; i < strlen(string); i++)
		{
			xQueueSendToBack(UART_QueueTransmitHandle[target], string + i, portMAX_DELAY); // send character to queue
		}

		xSemaphoreGive(UART_MutexTransmitHandle[target]);
	}
}
