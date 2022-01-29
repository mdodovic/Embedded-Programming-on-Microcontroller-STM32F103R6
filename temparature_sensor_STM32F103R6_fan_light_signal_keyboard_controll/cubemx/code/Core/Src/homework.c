/*
 * homework.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#include "homework.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>
#include <string.h>

#include "driver_lcd.h"
#include "driver_uart.h"
#include "driver_motor.h"
#include "driver_temp.h"
#include "driver_keypad.h"

#include "gpio.h"

#include "timers.h"
#include "tim.h"

//FanState fanState = TURNED_OFF;

char tempText[4];
uint32_t tempValue;

TimerHandle_t TimerHandler;



void homeworkTask(void *p)
{
	char message[] = "Temparatura: ";
	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00);

	for(uint32_t i = 0; i < strlen(message); i++)
	{
		LCD_CommandEnqueue(LCD_DATA, message[i]);
		//UART_AsyncTransmitCharacter(message[i]);
	}
	UART_AsyncTransmitString("Podeoci: ");

	while(1)
	{
		tempValue = TEMP_GetCurrentValue();
		itoa(tempValue, tempText, 10);


//		FanState desiredFanState;
		if(tempValue < 25)
		{
			htim3.Instance->CCR1 = 0;
//          desiredFanState = TURNED_OFF;
		}
		else if(tempValue < 35)
		{
//			desiredFanState = VERY_SLOW;

			uint32_t speedStep = (35-25) / (STEP - 2);
			for(uint32_t i = 0; i < STEP; i ++)
			{
				if(tempValue > i * speedStep + 25 && tempValue <= (i + 1) * speedStep + 25)
				{
					htim3.Instance->CCR1 = (i+1) * INCREMENT;
				}
			}
		}
//		else if(tempValue < 35)
//		{
//			desiredFanState = SLOW;
//		}
		else
		{
			htim3.Instance->CCR1 = 10;
// 			desiredFanState = FAST;
		}

//		for(uint32_t i = 0; i < abs(fanState - desiredFanState); i++)
//		{
//			if(fanState > desiredFanState)
//			{
//				MOTOR_SpeedDecrease();
//			}
//			else
//			{
//				MOTOR_SpeedIncrease();
//			}
//		}
//		fanState = desiredFanState;

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x0D);

		for(uint32_t i = 0; i < strlen(tempText); i++)
		{
			LCD_CommandEnqueue(LCD_DATA, tempText[i]);
			//UART_AsyncTransmitCharacter(tempText[i]);
		}

		if(5 < tempValue && tempValue <= 18)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // Blue
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		}
		if(18 < tempValue && tempValue <= 25)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Green
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		}
		if(25 < tempValue && tempValue <= 30)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // Yellow
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		}
		if(30 < tempValue && tempValue < 40)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // Red
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		}

		vTaskDelay(pdMS_TO_TICKS(200));

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x0D);

		for(uint32_t i = 0; i < strlen(tempText); i++)
		{
			LCD_CommandEnqueue(LCD_DATA, ' ');
			//UART_AsyncTransmitCharacter('\b');
		}

	}

}

void TimerToggle(TimerHandle_t xTimer)
{
	if(tempValue >= 40)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12); // Red
	}
	if(tempValue <= 35)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15); // Blue
	}
}

void homeworkInit()
{

	TimerHandler = xTimerCreate("timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, TimerToggle);
	xTimerStart(TimerHandler, portMAX_DELAY);

	LCD_Init();
	UART_Init();
	MOTOR_Init();
	TEMP_Init();
	KEY_Init();

	xTaskCreate(homeworkTask, "homeworkTask", 64, NULL, 5, NULL);
}

