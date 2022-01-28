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

	}

}

void MCU2_Init()
{

	LCD_Init();
	UART_Init();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	xTaskCreate(MCU_Task, "MCU_Task", 64, NULL, 5, NULL);

}


