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


void exampleInit()
{
	xTaskCreate(exampleTaskFunction, "exampleTask0", 128, (void*) GPIO_PIN_0, 2, NULL);
	xTaskCreate(exampleTaskFunction, "exampleTask1", 128, (void*) GPIO_PIN_1, 1, NULL);
}

void exampleTaskFunction(void* parameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOC, (uint32_t) parameters);
		vTaskDelay(1000);
	}
}
