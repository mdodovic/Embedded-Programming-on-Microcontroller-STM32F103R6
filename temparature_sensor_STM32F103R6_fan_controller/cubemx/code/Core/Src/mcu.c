/*
 * mcu.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "mcu.h"
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "uart_driver.h"
#include "temparature_driver.h"
#include "lcd_driver.h"
#include "motor_driver.h"

static uint32_t current_temparature;
static char current_temparature_text[4];

FanState fanState = TURNED_OFF;


void MCU_Task(void* p)
{

	// Create celsius sign

	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_CG_RAM_ADDRESS_INSTRUCTION | 0x38); // set cursor to the 111|000 symbol (address for this character)

	LCD_CommandEnqueue(LCD_DATA, 0x18); // X X _ _ _
	LCD_CommandEnqueue(LCD_DATA, 0x18); // X X _ _ _
	LCD_CommandEnqueue(LCD_DATA, 0x06); // _ _ X X _
	LCD_CommandEnqueue(LCD_DATA, 0x08); // _ X _ _ _
	LCD_CommandEnqueue(LCD_DATA, 0x08); // _ X _ _ _
	LCD_CommandEnqueue(LCD_DATA, 0x08); // _ X _ _ _
	LCD_CommandEnqueue(LCD_DATA, 0x06); // _ _ X X _

	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x0F); // set cursor to the celsius position
	LCD_CommandEnqueue(LCD_DATA, 0x07); // read from 111 address


	const char temparature[] = "Temparatura: ";

	// LCD
	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00); // set cursor to the row start
	for(uint32_t i = 0; i < strlen(temparature); i++)
	{
		LCD_CommandEnqueue(LCD_DATA, temparature[i]);
	}

	// Terminal
	UART_AsyncTransmitString(temparature);


	while(1)
	{

		current_temparature = TEMP_GetCurrentTemparatureValue();
		itoa(current_temparature, current_temparature_text, 10);

		// process temparature
		FanState desiredFanState;
		if(current_temparature < 30)
		{
			desiredFanState = TURNED_OFF;
		}
		else if(current_temparature < 35)
		{
			desiredFanState = SLOW;
		} else
		{
			desiredFanState = FAST;
		}

		for(uint32_t i = 0; i < abs(fanState - desiredFanState); i++)
		{
			if(desiredFanState > fanState)
			{
				MOTOR_SpeedIncrease();
			} else
			{
				MOTOR_SpeedDecrease();
			}
		}
		fanState = desiredFanState;

		// write new value

		// LCD
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x0D); // set cursor to the number position (= 14 == 0xd)
		for(uint32_t i = 0; i < strlen(current_temparature_text); i++)
		{
			LCD_CommandEnqueue(LCD_DATA, current_temparature_text[i]);
		}

		// Terminal
		UART_AsyncTransmitString(current_temparature_text);


		vTaskDelay(pdMS_TO_TICKS(200));


		// prepare for next value show (clear on display and on terminal)

		// LCD
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x0D); // set cursor to the number position (= 14 == 0xd)
		for(uint32_t i = 0; i < strlen(current_temparature_text); i++)
		{
			LCD_CommandEnqueue(LCD_DATA, ' ');
		}
		// Terminal
		for(uint32_t i = 0; i < strlen(current_temparature_text); i++)
		{
			UART_AsyncTransmitString("\b");
		}


	}
}



void MCU_Init()
{
	UART_Init();
	LCD_Init();
	MOTOR_Init();
	TEMP_Init();
	xTaskCreate(MCU_Task, "MCU_Task", 128, NULL, 5, NULL);

}
