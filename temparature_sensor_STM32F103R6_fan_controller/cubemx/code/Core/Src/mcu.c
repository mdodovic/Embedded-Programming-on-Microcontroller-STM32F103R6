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


static uint32_t current_temparature;
static char current_temparature_text[4];


void MCU_Task(void* p)
{
	const char temparature[] = "Temparatura: ";

	// LCD
	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00); // set cursor to the row start
	for(uint32_t i = 0; i < strlen(temparature); i++)
	{
		LCD_CommandEnqueue(LCD_DATA, temparature[i]);
	}

	// Terminal
	UART_AsyncTransmitString("Temparatura: ");


	while(1)
	{

		current_temparature = TEMP_GetCurrentTemparatureValue();
		itoa(current_temparature, current_temparature_text, 10);

		UART_AsyncTransmitString(current_temparature_text);


		current_temparature = TEMP_GetCurrentTemparatureValue();
	//	UART_AsyncTransmitString("MIPS\r");
		vTaskDelay(pdMS_TO_TICKS(200));

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


	xTaskCreate(MCU_Task, "MCU_Task", 128, NULL, 5, NULL);

}
