/*
 * driver_keypad.c
 *
 *  Created on: Jan 29, 2022
 *      Author: matij
 */

#include "driver_keypad.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

#include "gpio.h"

#include "driver_uart.h"
#include "driver_motor.h"

static char KEY_Map[4][3] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'},
};

uint8_t KEY_PreviouslyReleased = 1;

TimerHandle_t KEY_TimerHandle;
TaskHandle_t KEY_TaskHandle;

void KEY_Task()
{
	while(1){

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(KEY_PreviouslyReleased)
		{


			for(uint32_t row = 0; row < 4; row++)
			{
//				GPIOB->ODR = (0x01 << row);

				switch (row) {
					case 0:
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
						break;
					case 1:
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
						break;
					case 2:
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
						break;
					case 3:
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
						break;
				}


				uint8_t state = ((GPIOB->IDR) >> 4) & 0x7;

				for(uint32_t column = 0; column < 3; column++)
				{
					if(state & (0x01 << column))
					{
						KEY_PreviouslyReleased = 0;
						xTimerStart(KEY_TimerHandle, portMAX_DELAY);
						UART_AsyncTransmitCharacter(KEY_Map[row][column]);
						STEP = KEY_Map[row][column] - '0';
						INCREMENT = (9 + 1) / STEP;
					}
				}
			}

//			GPIOB->ODR = 0xF;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);

		}
	}
}

void KEY_Timer(TimerHandle_t xTimer)
{
	if(!KEY_PreviouslyReleased)
	{
		if((((GPIOB->IDR) >> 4) & 0x07) == 0)
		{
			KEY_PreviouslyReleased = 1;
		}
		else
		{
			xTimerStart(KEY_TimerHandle, portMAX_DELAY);
		}
	}

}


void KEY_Init()
{

//	GPIOB->ODR = 0xF;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);



	KEY_TimerHandle = xTimerCreate("keyTimer", pdMS_TO_TICKS(20), pdFALSE, NULL, KEY_Timer);
	xTaskCreate(KEY_Task, "KEY_Task", 64, NULL, 26, &KEY_TaskHandle);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_7)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		vTaskNotifyGiveFromISR(KEY_TaskHandle, &xHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
