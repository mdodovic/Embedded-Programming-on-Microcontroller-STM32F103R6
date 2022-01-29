/*
 * driver_temp.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "adc.h"


#define MAX_VOLTAGE 5.0
#define RESOLUTION 4096.0


QueueHandle_t TEMP_QueueHandle;
TaskHandle_t TEMP_TaskHandle;

void TEMP_Task(void* p)
{
	while(1)
	{
		HAL_ADC_Start_IT(&hadc1);

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		float value = HAL_ADC_GetValue(&hadc1);
		value = value * MAX_VOLTAGE / RESOLUTION;
		value = value * 100;

		xQueueOverwrite(TEMP_QueueHandle, &value);
		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == hadc1.Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(TEMP_TaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}

void TEMP_Init()
{
	TEMP_QueueHandle = xQueueCreate(1, sizeof(float));
	xTaskCreate(TEMP_Task, "TEMP_Task", 64, NULL, 2, &TEMP_TaskHandle);


}

float TEMP_GetCurrentValue()
{
	float temp = 0.0;
	xQueuePeek(TEMP_QueueHandle, &temp, portMAX_DELAY);
	return temp;
}
