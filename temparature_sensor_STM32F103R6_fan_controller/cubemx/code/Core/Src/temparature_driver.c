/*
 * temparature_driver.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "temparature_driver.h"
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "adc.h"

#define MAX_VOLTAGE 5.0
#define RESOLUTION 4096

static TaskHandle_t TEMP_TaskHandle;
static QueueHandle_t TEMP_QueueHandle;



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == hadc1.Instance)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(TEMP_TaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}

void TEMP_Task(void* p)
{
	while(1)
	{
		HAL_ADC_Start_IT(&hadc1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);


		float value = HAL_ADC_GetValue(&hadc1); // get raw conversion result
		value *= MAX_VOLTAGE / RESOLUTION; // translate into the voltage value
		value *= 100; // translate into temperature value (problem specific)

		xQueueOverwrite(TEMP_QueueHandle, &value);
		vTaskDelay(pdMS_TO_TICKS(200));
	}
}


float TEMP_GetCurrentTemparatureValue()
{
	float current_temparature = 0.0;

	xQueuePeek(TEMP_QueueHandle, &current_temparature, portMAX_DELAY);

	return current_temparature;
}



void TEMP_Init()
{
	TEMP_QueueHandle = xQueueCreate(1, sizeof(float));
	xTaskCreate(TEMP_Task, "TEMP_Task", 64, NULL, 2, &TEMP_TaskHandle);
}
