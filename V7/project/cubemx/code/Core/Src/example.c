/*
 * example.c
 *
 *  Created on: Jan 23, 2022
 *      Author: matij
 */

#include "example.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"


void exampleTaskFunction(void* parameters);
void taskController(void* parameters);
void taskToggler(void* parameters);

TaskHandle_t controllerHandle;
TaskHandle_t togglerHandle;


void exampleInit()
{
	xTaskCreate(taskController, "controller", 128, NULL, 2, &controllerHandle);
	xTaskCreate(taskToggler, "toggler", 128, NULL, 2, &togglerHandle);
}

void taskController(void* parameters)
{
	while(1)
	{
		xTaskNotifyGive(togglerHandle);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}

void taskToggler(void* parameters)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	}
}


void exampleTaskFunction(void* parameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOC, (uint32_t) parameters);
//		vTaskDelay(1000);

		for(int i = 0; i < 1000000; i++);
	}
}



