/*
 * mcu2.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "mcu2.h"

#include "FreeRTOS.h"
#include "task.h"

#include "tim.h"

#include "lcd_driver.h"
#include "usart_driver.h"

#include "gpio.h"
#include "timers.h"

TimerHandle_t MCU2_TimerTask;

void MCU_Task(void* p)
{

	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_CG_RAM_ADDRESS_INSTRUCTION | 0x08);

	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // XXXXX

	htim3.Instance->CCR1 = 0;
	htim3.Instance->CCR2 = 0;

	while(1)
	{
		MotorCommand motorCommand = UART_BlockReceiveMotorCommand();
		switch (motorCommand.motor)
		{
		case 1:
			htim3.Instance->CCR1 = motorCommand.velocity;
			LCD_CommandEnqueue(LCD_INSTRUCTION,
			LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00);
			break;
		case 2:
			htim3.Instance->CCR2 = motorCommand.velocity;
			LCD_CommandEnqueue(LCD_INSTRUCTION,
			LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x40);
			break;
		}

		for (uint32_t i = 0; i < 16; i++)
		{
			if (i < motorCommand.velocity)
			{
				LCD_CommandEnqueue(LCD_DATA, 0x01);
			}
			else
			{
				LCD_CommandEnqueue(LCD_DATA, ' ');
			}
		}
		uint32_t v1 = htim3.Instance->CCR1;
		uint32_t v2 = htim3.Instance->CCR2;


		if(v1 + v2 == 0)
		{
			xTimerStart(MCU2_TimerTask, portMAX_DELAY);
		}
		else if(1 <= v1 + v2 && v1 + v2 <= 3)
		{
			// red
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		else if(4 <= v1 + v2 && v1 + v2 <= 7)
		{
			// yellow
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		else if(8 <= v1 + v2 && v1 + v2 <= 10)
		{
			// blue
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		}


	}

}

void MCU2_TimerCallback(TimerHandle_t xTimer)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
	uint32_t v1 = htim3.Instance->CCR1;
	uint32_t v2 = htim3.Instance->CCR2;
	if(v1 + v2 != 0)
	{
		xTimerStop(xTimer, 0);
	}

}

void MCU2_Init()
{
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

	LCD_Init();
	UART_Init();

	MCU2_TimerTask = xTimerCreate("tim", pdMS_TO_TICKS(500), pdTRUE, NULL, MCU2_TimerCallback);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	xTaskCreate(MCU_Task, "MCU_Task", 64, NULL, 5, NULL);

}


