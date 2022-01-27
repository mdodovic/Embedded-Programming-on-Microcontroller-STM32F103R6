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

static void mcu1Task(void *p)
{

	LCD_CommandEnqueue(LCD_INSTRUCTION,
						LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00); // reset ddram showing

	char line1[16] = "Mikroprocesorski";

	for(uint32_t i = 0; i < 16; i++)
	{
		LCD_CommandEnqueue(LCD_DATA, line1[i]);
		vTaskDelay(pdMS_TO_TICKS(100));
	}

	while(1)
	{

	}
}


void MCU2_Init()
{

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	LCD_Init();

	xTaskCreate(mcu1Task, "mcu1Task", 128, NULL, 5, NULL);
}


