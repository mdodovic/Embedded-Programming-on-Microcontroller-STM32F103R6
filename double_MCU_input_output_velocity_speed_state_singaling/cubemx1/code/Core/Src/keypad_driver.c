/*
 * keypad_driver.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "keypad_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "uart_driver.h"
#include "mcu1.h"

#include "gpio.h"

TaskHandle_t KEY_TaskHandle;
TimerHandle_t KEY_TimerHandle;

uint8_t KEY_PreviouslyReleased = 1;

static char KEY_Map[4][3] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'},
};

uint8_t number = 0;

static uint8_t velocityKeyboard = -1;
static uint8_t motorKeyboard = 0;

void KEY_Task(void* p)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if(KEY_PreviouslyReleased)
		{
			for(uint8_t row = 0; row < 4; row++)
			{
				GPIOB->ODR = 0x01 << row;

				uint32_t state = ((GPIOB->IDR) >> 4) & 0x07;

				for(uint8_t column = 0; column < 3; column++)
				{
					if(state & (0x01 << column))
					{
						KEY_PreviouslyReleased = 0;
						xTimerStart(KEY_TimerHandle, portMAX_DELAY);

						if(KEY_Map[row][column] == '#')
						{
							motorKeyboard = 1;
							velocityKeyboard = 14;

							MotorCommand motorCommand =
							{ .motor = motorKeyboard, .velocity = velocityKeyboard};
							UART_AsyncTransmitxMotorCommand(MCU2, motorCommand);

							velocity[motorKeyboard - 1] = velocityKeyboard;
							motorKeyboard = -1;
							velocityKeyboard = 0;
						}
						else
						{
							uint8_t number = KEY_Map[row][column] - '0';
							UART_AsyncTransmitDecimal(VT, number);

							if(motorKeyboard == -1)
							{
								motorKeyboard = number;
							} else
							{
								velocityKeyboard = velocityKeyboard * 10 + number;
							}

						}

					}
				}
			}

			GPIOB->ODR = 0xF;

		}

	}
}


void KEY_TimerCallback(TimerHandle_t xTimer)
{
	if(!KEY_PreviouslyReleased)
	{
		if(((GPIOB->IDR >> 4) & 0x7) == 0)
		{
			KEY_PreviouslyReleased = 1;
		}
		else {
			xTimerStart(KEY_TimerHandle, portMAX_DELAY);
		}
	}
}

void KEY_Init()
{
	GPIOB->ODR = 0xF;

	KEY_TimerHandle = xTimerCreate("KEY_TimerHandle", pdMS_TO_TICKS(50), pdFALSE, NULL, KEY_TimerCallback);

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



