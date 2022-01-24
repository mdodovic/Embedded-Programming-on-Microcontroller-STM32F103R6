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
#include "timers.h"

void exampleTaskFunction(void* parameters);
void taskController(void* parameters);
void taskToggler(void* parameters);
void defferedTask(void* parameters);

TaskHandle_t controllerHandle;
TaskHandle_t togglerHandle;
TaskHandle_t defferedHandle;

void callbackDeffered(void* p1, uint32_t p2);


void exampleInit()
{
	//xTaskCreate(taskController, "controller", 128, NULL, 2, &controllerHandle);
	//xTaskCreate(taskToggler, "toggler", 128, NULL, 2, &togglerHandle);
	//xTaskCreate(defferedTask, "defferedTask", 128, NULL, 2, &defferedHandle);


}


void callbackDeffered(void* p1, uint32_t p2)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		BaseType_t woken = pdFALSE;
		xTimerPendFunctionCallFromISR(callbackDeffered, NULL, 0, &woken);
//		vTaskNotifyGiveFromISR(defferedHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}


void defferedTask(void* parameters)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	}
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



