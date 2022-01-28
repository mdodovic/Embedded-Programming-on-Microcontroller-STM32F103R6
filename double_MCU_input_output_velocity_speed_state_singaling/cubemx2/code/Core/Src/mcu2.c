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


	while(1)
	{

		uint32_t vel = htim3.Instance->CCR1;
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00);

		for(uint32_t i = 0; i < 16; i++)
		{
			if(i < vel){
				LCD_CommandEnqueue(LCD_DATA, 0x01);
			} else {
				LCD_CommandEnqueue(LCD_DATA, ' ');
			}
		}

		vel = htim3.Instance->CCR2;
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x40);

		for(uint32_t i = 0; i < 16; i++)
		{
			if(i < vel){
				LCD_CommandEnqueue(LCD_DATA, 0x01);
			} else {
				LCD_CommandEnqueue(LCD_DATA, ' ');
			}
		}

		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}

void MCU2_Init()
{

	LCD_Init();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	xTaskCreate(MCU_Task, "MCU_Task", 64, NULL, 5, NULL);

}


