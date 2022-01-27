/*
 * mcu2.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "mcu2.h"

#include "FreeRTOS.h"
#include "task.h"


#include "tim.h"

#include "lcd.h"

#include "uart_driver.h"

static void mcu1Task(void *p)
{

	LCD_CommandEnqueue(LCD_INSTRUCTION,
						LCD_SET_CG_RAM_ADDRESS_INSTRUCTION | 0x08);

	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X
	LCD_CommandEnqueue(LCD_DATA, 0x1F); // X X X X X


	LCD_CommandEnqueue(LCD_INSTRUCTION,
						LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00); // reset ddram showing

	MotorCommand mc;
	while(1)
	{

		mc = UART_BlockReceiveMotorCommand();

		if(mc.motor == 0)
		{
			htim3.Instance->CCR1 = mc.velocity;
			LCD_CommandEnqueue(LCD_INSTRUCTION,
								LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00); // first row
		}
		else
		{
			htim3.Instance->CCR2 = mc.velocity;
			LCD_CommandEnqueue(LCD_INSTRUCTION,
								LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x40); // second row
		}

		for(uint32_t i = 0; i < 16; i++)
		{
			if(i < mc.velocity)
			{
				LCD_CommandEnqueue(LCD_DATA, 0x01); // read from cgram
			}
			else
			{
				LCD_CommandEnqueue(LCD_DATA, ' '); // fill with blanco
			}
		}

	}
}


void MCU2_Init()
{

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	LCD_Init();
	UART_Init();
	xTaskCreate(mcu1Task, "mcu1Task", 128, NULL, 5, NULL);
}


