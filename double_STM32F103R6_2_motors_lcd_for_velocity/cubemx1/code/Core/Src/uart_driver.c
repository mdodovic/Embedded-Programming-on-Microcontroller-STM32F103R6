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

		HAL_UART_Transmit_IT(phuart[target], &buffer, sizeof(uint8_t));	 // transmit it to the pin TX

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until it is transmitted

	}
}

// callback after the transmission is completed

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// transmission finished: next transmission can start
	if(huart->Instance == phuart[VT]->Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_TaskTransmitHandle[VT], &woken);
		portYIELD_FROM_ISR(woken);
	}
	if(huart->Instance == phuart[MCU2]->Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_TaskTransmitHandle[MCU2], &woken);
		portYIELD_FROM_ISR(woken);
	}
}

/* Receive */

// task for receiving

static void UART_ReceiveTask(void* p)
{
	UART_Target target = (UART_Target) p;
	uint8_t buffer;

	while(1)
	{

		HAL_UART_Receive_IT(phuart[target], &buffer, sizeof(uint8_t)); // initiate receiving

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until it is received

		xQueueSendToBack(UART_QueueReceiveHandle[target], &buffer, portMAX_DELAY); // send character to queue

	}
}

// callback after the receive is completed

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// receive finished: character can be consumed
	if(huart->Instance == phuart[VT]->Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_TaskReceiveHandle[VT], &woken);
		portYIELD_FROM_ISR(woken);
	}


}


void UART_Init()
{
	/* Transmit to Virtual Terminal */

	UART_QueueTransmitHandle[VT] = xQueueCreate(128, sizeof(uint8_t));
	UART_MutexTransmitHandle[VT] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void *) VT, 4, &UART_TaskTransmitHandle[VT]);

	/* Receive from Virtual Terminal */

	UART_QueueReceiveHandle[VT] = xQueueCreate(128, sizeof(uint8_t));
	UART_MutexReceiveHandle[VT] = xSemaphoreCreateMutex();
	xTaskCreate(UART_ReceiveTask, "UART_ReceiveTask", 64, (void *) VT, 20, &UART_TaskReceiveHandle[VT]);

	/* Transmit to MCU2 */
	UART_QueueTransmitHandle[MCU2] = xQueueCreate(128, sizeof(uint8_t));
	UART_MutexTransmitHandle[MCU2] = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "UART_TransmitTask", 64, (void*) MCU2, 4, &UART_TaskTransmitHandle[MCU2]);
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

void UART_AsyncTransmitMotorCommand(UART_Target target, MotorCommand command)
{

		xSemaphoreTake(UART_MutexTransmitHandle[target], portMAX_DELAY);

		xQueueSendToBack(UART_QueueTransmitHandle[target], &command.motor, portMAX_DELAY);
		xQueueSendToBack(UART_QueueTransmitHandle[target], &command.velocity, portMAX_DELAY);

		xSemaphoreGive(UART_MutexTransmitHandle[target]);
}


/* utility: Receive */

char* UART_BlockReceiveString(UART_Target target)
{
	xSemaphoreTake(UART_MutexReceiveHandle[target], portMAX_DELAY);

	char* string = pvPortMalloc(32);

	if(string != NULL)
	{

		uint32_t i = 0;
		char c = '\0';
		while(c != '\r' && i < 32)
		{
			xQueueReceive(UART_QueueReceiveHandle[target], &c, portMAX_DELAY); // fetch character from string
			string[i++] = c;
		}
		string[i--] = '\0';
	}
	xSemaphoreGive(UART_MutexReceiveHandle[target]);

	return string;
}

MotorCommand UART_BlockReceiveMotorCommand(UART_Target target)
{
	MotorCommand mc = {0, 0};
	xSemaphoreTake(UART_MutexTransmitHandle[target], portMAX_DELAY);

	xQueueReceive(UART_QueueReceiveHandle[target], &mc.motor, portMAX_DELAY);
	xQueueReceive(UART_QueueReceiveHandle[target], &mc.velocity, portMAX_DELAY);

	xSemaphoreGive(UART_MutexTransmitHandle[target]);
	return mc;
}
