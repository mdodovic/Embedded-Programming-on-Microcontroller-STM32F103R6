/*
 * leypad.c
 *
 *  Created on: Jan 25, 2022
 *      Author: matij
 */


#include "keypad.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "task.h"

#include "lcd.h"

static char KEY_Map[4][3] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'},
};

TaskHandle_t KEY_TaskHandle;

static void KEY_Task(void* parameters)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		for(uint8_t row = 0; row < 4; row++)
		{
			GPIOB->ODR = 0x01 << row;

			// stable signal

			uint32_t state = (((GPIOB->IDR) >> 4) &0x7);

			for(uint8_t column = 0; column < 3; column++)
			{
				if(state && (0x01 << column))
				{
					LCD_CommandEnqueue(LCD_DATA, KEY_Map[row][column]);
				}
			}

		}

		GPIOB->ODR = 0x0F;

	}
}

void KEY_Init()
{
	GPIOB->ODR = 0x0F;
	xTaskCreate(KEY_Task, "KEY Task", 128, NULL, 10, &KEY_TaskHandle);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_7)
	{
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(KEY_TaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}
