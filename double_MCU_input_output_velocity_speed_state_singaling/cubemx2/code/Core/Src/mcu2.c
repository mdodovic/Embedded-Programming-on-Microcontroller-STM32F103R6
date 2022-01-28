/*
 * mcu2.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "mcu2.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lcd_driver.h"



void MCU_Task(void* p)
{


	while(1)
	{
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x40);

		LCD_CommandEnqueue(LCD_DATA, 'a');

		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}

void MCU2_Init()
{

	LCD_Init();

	xTaskCreate(MCU_Task, "MCU_Task", 64, NULL, 5, NULL);

}


